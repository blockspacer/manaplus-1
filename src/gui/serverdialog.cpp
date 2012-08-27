/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gui/serverdialog.h"

#include "chatlog.h"
#include "client.h"
#include "configuration.h"
#include "gui.h"
#include "log.h"
#include "main.h"

#include "gui/login.h"
#include "gui/okdialog.h"
#include "gui/sdlinput.h"
#include "gui/theme.h"

#include "gui/widgets/button.h"
#include "gui/widgets/dropdown.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/textfield.h"

#include "net/net.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"
#include "widgets/dropdown.h"

#include <guichan/font.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

static const int MAX_SERVERLIST = 15;

static std::string serverTypeToString(ServerInfo::Type type)
{
    switch (type)
    {
        case ServerInfo::TMWATHENA:
            return "TmwAthena";
#ifdef MANASERV_SUPPORT
        case ServerInfo::MANASERV:
            return "ManaServ";
#endif
        default:
        case ServerInfo::UNKNOWN:
            return "";
    }
}

static unsigned short defaultPortForServerType(ServerInfo::Type type)
{
    switch (type)
    {
        default:
        case ServerInfo::UNKNOWN:
        case ServerInfo::TMWATHENA:
            return 6901;
#ifdef MANASERV_SUPPORT
        case ServerInfo::MANASERV:
            return 9601;
#endif
    }
}

ServersListModel::ServersListModel(ServerInfos *servers, ServerDialog *parent):
        mServers(servers),
        mVersionStrings(servers->size(), VersionString(0, "")),
        mParent(parent)
{
}

int ServersListModel::getNumberOfElements()
{
    MutexLocker lock = mParent->lock();
    return static_cast<int>(mServers->size());
}

std::string ServersListModel::getElementAt(int elementIndex)
{
    MutexLocker lock = mParent->lock();
    const ServerInfo &server = mServers->at(elementIndex);
    std::string myServer;
    myServer += server.hostname;
    myServer += ":";
    myServer += toString(server.port);
    return myServer;
}

void ServersListModel::setVersionString(int index, const std::string &version)
{
    if (version.empty())
    {
        mVersionStrings[index] = VersionString(0, "");
    }
    else
    {
        int width = gui->getFont()->getWidth(version);
        mVersionStrings[index] = VersionString(width, version);
    }
}

std::string TypeListModel::getElementAt(int elementIndex)
{
    if (elementIndex == 0)
        return "TmwAthena";
#ifdef MANASERV_SUPPORT
    else if (elementIndex == 1)
        return "ManaServ";
#endif
    else
        return "Unknown";
}

class ServersListBox : public ListBox
{
public:
    ServersListBox(ServersListModel *model):
            ListBox(model)
    {
    }

    void draw(gcn::Graphics *graphics)
    {
        if (!mListModel)
            return;

        ServersListModel *model = static_cast<ServersListModel*>(mListModel);

        updateAlpha();

        graphics->setColor(Theme::getThemeColor(Theme::HIGHLIGHT,
                static_cast<int>(mAlpha * 255.0f)));
        graphics->setFont(getFont());

        const int height = getRowHeight();
        const gcn::Color unsupported =
                Theme::getThemeColor(Theme::SERVER_VERSION_NOT_SUPPORTED,
                                     static_cast<int>(mAlpha * 255.0f));

        // Draw filled rectangle around the selected list element
        if (mSelected >= 0)
        {
            graphics->fillRectangle(gcn::Rectangle(0, height * mSelected,
                                                   getWidth(), height));
        }

        // Draw the list elements
        for (int i = 0, y = 0; i < model->getNumberOfElements();
             ++i, y += height)
        {
            ServerInfo info = model->getServer(i);

            graphics->setColor(Theme::getThemeColor(Theme::TEXT));

            int top;

            if (!info.name.empty())
            {
                graphics->setFont(boldFont);
                graphics->drawText(info.name, 2, y);
                top = y + height / 2;
            }
            else
            {
                top = y + height / 4;
            }

            graphics->setFont(getFont());

            graphics->drawText(model->getElementAt(i), 2, top);

            if (info.version.first > 0)
            {
                graphics->setColor(unsupported);

                graphics->drawText(info.version.second,
                                   getWidth() - info.version.first - 2, top);
            }
        }
    }

    unsigned int getRowHeight() const
    {
        return 2 * getFont()->getHeight();
    }
};


ServerDialog::ServerDialog(ServerInfo *serverInfo, const std::string &dir):
    Window(_("Choose Your Server")),
    mDir(dir),
//    mDownloadStatus(DOWNLOADING_PREPARING),
    mDownloadStatus(DOWNLOADING_UNKNOWN),
    mDownload(0),
    mDownloadProgress(-1.0f),
    mServers(ServerInfos()),
    mServerInfo(serverInfo),
    mPersistentIPCheckBox(0)
{
    if (isSafeMode)
        setCaption("Choose Your Server  *** SAFE MODE ***");

    setWindowName("ServerDialog");

    Label *serverLabel = new Label(_("Server:"));
    Label *portLabel = new Label(_("Port:"));
    mServerNameField = new TextField(mServerInfo->hostname);
    mPortField = new TextField(toString(mServerInfo->port));
    mPersistentIPCheckBox = new CheckBox(_("Use same ip for game sub servers"),
                                       config.getBoolValue("usePersistentIP"),
                                       this, "persitent ip");

    loadCustomServers();

    mServersListModel = new ServersListModel(&mServers, this);

    mServersList = new ServersListBox(mServersListModel);
    mServersList->addMouseListener(this);

    ScrollArea *usedScroll = new ScrollArea(mServersList);
    usedScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);

#ifdef MANASERV_SUPPORT
    Label *typeLabel = new Label(_("Server type:"));
    mTypeListModel = new TypeListModel();
    mTypeField = new DropDown(mTypeListModel);
    mTypeField->setSelected((serverInfo->type == ServerInfo::MANASERV) ?
                            1 : 0);
    int n = 1;
#else
    mTypeListModel = 0;
    mTypeField = 0;
    int n = 0;
#endif

    mDescription = new Label(std::string());

    mQuitButton = new Button(_("Quit"), "quit", this);
    mLoadButton = new Button(_("Load"), "load", this);
    mConnectButton = new Button(_("Connect"), "connect", this);
    mManualEntryButton = new Button(_("Custom Server"), "addEntry", this);
    mDeleteButton = new Button(_("Delete"), "remove", this);

    mServerNameField->setActionEventId("connect");
    mPortField->setActionEventId("connect");

    mServerNameField->addActionListener(this);
    mPortField->addActionListener(this);
    mManualEntryButton->addActionListener(this);
    mServersList->addSelectionListener(this);
    usedScroll->setVerticalScrollAmount(0);

    place(0, 0, serverLabel);
    place(1, 0, mServerNameField, 5).setPadding(3);
    place(0, 1, portLabel);
    place(1, 1, mPortField, 5).setPadding(3);
#ifdef MANASERV_SUPPORT
    place(0, 2, typeLabel);
    place(1, 2, mTypeField, 5).setPadding(3);
#endif
    place(0, 2 + n, usedScroll, 6, 5).setPadding(3);
    place(0, 7 + n, mDescription, 6);
    place(0, 8 + n, mPersistentIPCheckBox, 6);
    place(0, 9 + n, mManualEntryButton);
    place(1, 9 + n, mDeleteButton);
    place(2, 9 + n, mLoadButton);
    place(4, 9 + n, mQuitButton);
    place(5, 9 + n, mConnectButton);

    // Make sure the list has enough height
    getLayout().setRowHeight(3, 80);

/*
    reflowLayout(400, 300);
    setDefaultSize(400, 300, ImageRect::CENTER);
*/
    // Do this manually instead of calling reflowLayout so we can enforce a
    // minimum width.
    int width = 0, height = 0;
    getLayout().reflow(width, height);
    if (width < 400)
    {
        width = 400;
        getLayout().reflow(width, height);
    }

    setContentSize(width, height);

    setMinWidth(getWidth());
    setMinHeight(getHeight());
    setDefaultSize(getWidth(), getHeight(), ImageRect::CENTER);

    setResizable(true);
    addKeyListener(this);

    loadWindowState();

    setFieldsReadOnly(true);
    mServersList->setSelected(0); // Do this after for the Delete button
    setVisible(true);

    if (mServerNameField->getText().empty())
    {
        mServerNameField->requestFocus();
    }
    else
    {
        if (mPortField->getText().empty())
            mPortField->requestFocus();
        else
            mConnectButton->requestFocus();
    }

    loadServers(false);

    if (mServers.size() == 0)
        downloadServerList();
}

ServerDialog::~ServerDialog()
{
    if (mDownload)
    {
        mDownload->cancel();
        delete mDownload;
        mDownload = 0;
    }
    delete mServersListModel;
    mServersListModel = 0;
    delete mTypeListModel;
    mTypeListModel = 0;
}

void ServerDialog::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "ok")
    {
        // Give focus back to the server dialog.
        mServerNameField->requestFocus();
    }
    else if (event.getId() == "connect")
    {
        // Check login
        if (mServerNameField->getText().empty()
            || mPortField->getText().empty())
        {
            OkDialog *dlg = new OkDialog(_("Error"),
                _("Please type both the address and the port of a server."));
            dlg->addActionListener(this);
        }
        else
        {
            if (mDownload)
                mDownload->cancel();

            mQuitButton->setEnabled(false);
            mConnectButton->setEnabled(false);
            mLoadButton->setEnabled(false);

            mServerInfo->hostname = mServerNameField->getText();
            mServerInfo->port = static_cast<short>(
                    atoi(mPortField->getText().c_str()));

            if (mTypeField)
            {
                switch (mTypeField->getSelected())
                {
                    case 0:
                        mServerInfo->type = ServerInfo::TMWATHENA;
                        break;
#ifdef MANASERV_SUPPORT
                    case 1:
                        mServerInfo->type = ServerInfo::MANASERV;
                        break;
#endif
                    default:
                        mServerInfo->type = ServerInfo::UNKNOWN;
                }
            }
            else
            {
                mServerInfo->type = ServerInfo::TMWATHENA;
            }

            // Save the selected server
            mServerInfo->save = true;

            if (chatLogger)
                chatLogger->setServerName(mServerInfo->hostname);

            saveCustomServers(*mServerInfo);

            if (!LoginDialog::savedPasswordKey.empty())
            {
                if (mServerInfo->hostname != LoginDialog::savedPasswordKey)
                    LoginDialog::savedPassword = "";
            }

            config.setValue("usePersistentIP",
                mPersistentIPCheckBox->isSelected());
            Client::setState(STATE_CONNECT_SERVER);
        }
    }
    else if (event.getId() == "quit")
    {
        if (mDownload)
            mDownload->cancel();
        Client::setState(STATE_FORCE_QUIT);
    }
    else if (event.getId() == "load")
    {
        downloadServerList();
    }
    else if (event.getId() == "addEntry")
    {
        setFieldsReadOnly(false);
    }
    else if (event.getId() == "remove")
    {
        int index = mServersList->getSelected();
        mServersList->setSelected(0);
        mServers.erase(mServers.begin() + index);

        saveCustomServers();
    }
}

void ServerDialog::keyPressed(gcn::KeyEvent &keyEvent)
{
    gcn::Key key = keyEvent.getKey();

    if (key.getValue() == Key::ESCAPE)
        Client::setState(STATE_EXIT);
    else if (key.getValue() == Key::ENTER)
        action(gcn::ActionEvent(NULL, mConnectButton->getActionEventId()));
}

void ServerDialog::valueChanged(const gcn::SelectionEvent &)
{
    const int index = mServersList->getSelected();
    if (index == -1)
    {
        mDeleteButton->setEnabled(false);
        return;
    }

    // Update the server and post fields according to the new selection
    const ServerInfo &myServer = mServersListModel->getServer(index);
    mDescription->setCaption(myServer.description);
    mServerNameField->setText(myServer.hostname);
    mPortField->setText(toString(myServer.port));
    if (mTypeField)
    {
        switch (myServer.type)
        {
            case ServerInfo::TMWATHENA:
            case ServerInfo::UNKNOWN:
            default:
                mTypeField->setSelected(0);
                break;
#ifdef MANASERV_SUPPORT
            case ServerInfo::MANASERV:
                mTypeField->setSelected(1);
                break;
#endif
        }
    }
    setFieldsReadOnly(true);

    mDeleteButton->setEnabled(myServer.save);
}

void ServerDialog::mouseClicked(gcn::MouseEvent &mouseEvent)
{
    if (mouseEvent.getClickCount() == 2 &&
        mouseEvent.getSource() == mServersList)
    {
        action(gcn::ActionEvent(mConnectButton,
                                mConnectButton->getActionEventId()));
    }
}

void ServerDialog::logic()
{
    {
        MutexLocker lock(&mMutex);
        if (mDownloadStatus == DOWNLOADING_COMPLETE)
        {
            mDownloadStatus = DOWNLOADING_OVER;

            mDescription->setCaption(std::string());
        }
        else if (mDownloadStatus == DOWNLOADING_IN_PROGRESS)
        {
            mDescription->setCaption(strprintf(_("Downloading server list..."
                "%2.2f%%"), mDownloadProgress * 100));
        }
        else if (mDownloadStatus == DOWNLOADING_IDLE)
        {
            mDescription->setCaption(_("Waiting for server..."));
        }
        else if (mDownloadStatus == DOWNLOADING_PREPARING)
        {
            mDescription->setCaption(_("Preparing download"));
        }
        else if (mDownloadStatus == DOWNLOADING_ERROR)
        {
            mDescription->setCaption(_("Error retreiving server list!"));
        }
    }

    Window::logic();
}

void ServerDialog::setFieldsReadOnly(bool readOnly)
{
    if (!readOnly)
    {
        mDescription->setCaption(std::string());
        mServersList->setSelected(-1);

        mServerNameField->setText(std::string());
        mPortField->setText(std::string("6901"));

        mServerNameField->requestFocus();
    }

    mManualEntryButton->setEnabled(readOnly);
    mDeleteButton->setEnabled(false);
    mLoadButton->setEnabled(readOnly);
    mDescription->setVisible(readOnly);

    mServerNameField->setEnabled(!readOnly);
    mPortField->setEnabled(!readOnly);
    if (mTypeField)
        mTypeField->setEnabled(!readOnly);
}

void ServerDialog::downloadServerList()
{
    // Try to load the configuration value for the onlineServerList
    std::string listFile = branding.getStringValue("onlineServerList");

    if (listFile.empty())
        listFile = config.getStringValue("onlineServerList");

    // Fall back to manasource.org when neither branding nor config set it
    if (listFile.empty())
        listFile = "http://manaplus.evolonline.org/serverlist.xml";

    mDownload = new Net::Download(this, listFile, &downloadUpdate);
    mDownload->setFile(mDir + "/serverlist.xml");
    mDownload->start();
}

void ServerDialog::loadServers(bool addNew)
{
    XML::Document doc(mDir + "/serverlist.xml", false);
    xmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "serverlist"))
    {
        logger->log1("Error loading server list!");
        return;
    }

    int version = XML::getProperty(rootNode, "version", 0);
    if (version != 1)
    {
        logger->log("Error: unsupported online server list version: %d",
                    version);
        return;
    }

    for_each_xml_child_node(serverNode, rootNode)
    {
        if (!xmlStrEqual(serverNode->name, BAD_CAST "server"))
            continue;

        ServerInfo server;

        std::string type = XML::getProperty(serverNode, "type", "unknown");

        server.type = ServerInfo::parseType(type);

        // Ignore unknown server types
        if (server.type == ServerInfo::UNKNOWN)
        {
            logger->log("Ignoring server entry with unknown type: %s",
                        type.c_str());
            continue;
        }

        server.name = XML::getProperty(serverNode, "name", std::string());

        std::string version = XML::getProperty(serverNode, "minimumVersion",
                                               std::string());

        bool meetsMinimumVersion = (compareStrI(version, PACKAGE_VERSION)
                                   <= 0);

        // For display in the list
        if (meetsMinimumVersion)
            version.clear();
        else if (version.empty())
            version = _("requires a newer version");
        else
            version = strprintf(_("requires v%s"), version.c_str());

        for_each_xml_child_node(subNode, serverNode)
        {
            if (xmlStrEqual(subNode->name, BAD_CAST "connection"))
            {
                server.hostname = XML::getProperty(subNode, "hostname", "");
                server.port = static_cast<short unsigned>(
                    XML::getProperty(subNode, "port", 0));

                if (server.port == 0)
                {
                    // If no port is given, use the default for the given type
                    server.port = defaultPortForServerType(server.type);
                }
            }
            else if (xmlStrEqual(subNode->name, BAD_CAST "description"))
            {
                server.description = reinterpret_cast<const char*>(
                    subNode->xmlChildrenNode->content);
            }
        }

        server.version.first = gui->getFont()->getWidth(version);
        server.version.second = version;

        MutexLocker lock(&mMutex);
        // Add the server to the local list if it's not already present
        bool found = false;
        for (unsigned int i = 0; i < mServers.size(); i++)
        {
            if (mServers[i] == server)
            {
                // Use the name listed in the server list
                mServers[i].name = server.name;
                mServers[i].version = server.version;
                mServersListModel->setVersionString(i, version);
                found = true;
                break;
            }
        }

        if (!found && addNew)
            mServers.push_back(server);
    }
}

void ServerDialog::loadCustomServers()
{
    for (int i = 0; i < MAX_SERVERLIST; ++i)
    {
        const std::string index = toString(i);
        const std::string nameKey = "MostUsedServerName" + index;
        const std::string typeKey = "MostUsedServerType" + index;
        const std::string portKey = "MostUsedServerPort" + index;

        ServerInfo server;
        server.hostname = config.getValue(nameKey, "");
        server.type = ServerInfo::parseType(config.getValue(typeKey, ""));

        const int defaultPort = defaultPortForServerType(server.type);
        server.port = static_cast<unsigned short>(
            config.getValue(portKey, defaultPort));

        // Stop on the first invalid server
        if (!server.isValid())
            break;

        server.save = true;
        mServers.push_back(server);
    }
}

void ServerDialog::saveCustomServers(const ServerInfo &currentServer)
{
    // Make sure the current server is mentioned first
    if (currentServer.isValid())
    {
        ServerInfos::iterator i, i_end = mServers.end();
        for (i = mServers.begin(); i != i_end; ++i)
        {
            if (*i == currentServer)
            {
                mServers.erase(i);
                break;
            }
        }
        mServers.insert(mServers.begin(), currentServer);
    }

    int savedServerCount = 0;

    for (unsigned i = 0;
         i < mServers.size() && savedServerCount < MAX_SERVERLIST; ++i)
    {
        const ServerInfo &server = mServers.at(i);

        // Only save servers that were loaded from settings
        if (!(server.save && server.isValid()))
            continue;

        const std::string index = toString(savedServerCount);
        const std::string nameKey = "MostUsedServerName" + index;
        const std::string typeKey = "MostUsedServerType" + index;
        const std::string portKey = "MostUsedServerPort" + index;

        config.setValue(nameKey, toString(server.hostname));
        config.setValue(typeKey, serverTypeToString(server.type));
        config.setValue(portKey, toString(server.port));
        ++savedServerCount;
    }

    // Insert an invalid entry at the end to make the loading stop there
    if (savedServerCount < MAX_SERVERLIST)
        config.setValue("MostUsedServerName" + toString(savedServerCount), "");
}

int ServerDialog::downloadUpdate(void *ptr, DownloadStatus status,
                                 size_t total, size_t remaining)
{
    if (status == DOWNLOAD_STATUS_CANCELLED)
        return -1;

    ServerDialog *sd = reinterpret_cast<ServerDialog*>(ptr);
    bool finished = false;

    if (!sd->mDownload)
        return -1;

    if (status == DOWNLOAD_STATUS_COMPLETE)
    {
        finished = true;
    }
    else if (status < 0)
    {
        logger->log("Error retreiving server list: %s\n",
                    sd->mDownload->getError());
        sd->mDownloadStatus = DOWNLOADING_ERROR;
    }
    else
    {
        float progress = static_cast<float>(remaining);
        if (total)
            progress /= static_cast<float>(total);

        if (progress != progress)
        {
            progress = 0.0f; // check for NaN
        }
        else if (progress < 0.0f)
        {
            progress = 0.0f; // no idea how this could ever happen,
                             // but why not check for it anyway.
        }
        else if (progress > 1.0f)
        {
            progress = 1.0f;
        }

        MutexLocker lock(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_IN_PROGRESS;
        sd->mDownloadProgress = progress;
    }

    if (finished)
    {
        sd->loadServers();

        MutexLocker lock(&sd->mMutex);
        sd->mDownloadStatus = DOWNLOADING_COMPLETE;
    }

    return 0;
}
