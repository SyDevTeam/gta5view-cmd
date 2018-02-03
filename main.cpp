/*****************************************************************************
* gta5view-cmd - gta5view Command Line
* Copyright (C) 2017 Syping
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "SnapmaticPicture.h"
#include <QCoreApplication>
#include <QImageReader>
#include <QDateTime>
#include <QPainter>
#include <QImage>
#include <QMap>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if (args.length() >= 3)
    {
        QString format = "jpg";
        bool isDefault = true;
        bool avatarMode = false;
        bool convertToGTA = false;
        bool customFormat = false;
        bool keepAspectRatio = true;
        QMap<QString,QString> flags;

        if (args.length() >= 4)
        {
            if (args.at(3) == "-pgta")
            {
                convertToGTA = true;
                customFormat = false;
            }
            else if (args.at(3) == "-g5e")
            {
                convertToGTA = true;
                customFormat = true;
            }
            else if (args.at(3) == "-jpg")
            {
                format = "jpg";
                convertToGTA = false;
            }
        }
        if (args.length() >= 5)
        {
            if (args.at(4) == "-a")
            {
                isDefault = false;
                avatarMode = true;
            }
            else if (args.at(4) == "-p")
            {
                isDefault = false;
                avatarMode = false;
            }
            else if (args.at(4) == "-d")
            {
                isDefault = true;
            }
            else if (args.at(4) == "-aiar")
            {
                isDefault = false;
                avatarMode = true;
                keepAspectRatio = false;
            }
            else if (args.at(4) == "-akar")
            {
                isDefault = false;
                avatarMode = true;
                keepAspectRatio = true;
            }
            else if (args.at(4) == "-piar")
            {
                isDefault = false;
                avatarMode = false;
                keepAspectRatio = false;
            }
            else if (args.at(4) == "-pkar")
            {
                isDefault = false;
                avatarMode = false;
                keepAspectRatio = true;
            }
            else if (args.at(4) == "-diar")
            {
                isDefault = true;
                keepAspectRatio = false;
            }
            else if (args.at(4) == "-dkar")
            {
                isDefault = true;
                keepAspectRatio = true;
            }
        }
        if (args.length() >= 6)
        {
            QStringList flagArgs(args);
            for (int argId = 0; argId == 5; argId++)
            {
                flagArgs.removeAt(argId);
            }
            for (QString flagArg : flagArgs)
            {
                QStringList currentFlagArg = flagArg.split("=");
                QString currentFlag = currentFlagArg.at(0).toLower();
                if (currentFlag.left(1) == "-")
                {
                    currentFlag.remove(0, 1);
                    currentFlagArg.removeAt(0);
                    QString currentValue = currentFlagArg.join(QString());
                    flags[currentFlag] = currentValue;
                }
            }
        }
        if (!convertToGTA)
        {
            SnapmaticPicture picture(args.at(1));
            if (picture.readingPicture(true, false, true))
            {
                QString filePath = args.at(2);
                filePath.replace("<autodef>", picture.getExportPictureFileName());
                filePath.replace("<autoext>", ".jpg");
                if (!picture.exportPicture(filePath, SnapmaticFormat::JPEG_Format))
                {
                    cout << "gta5view-cmd: Exporting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is failed!" << endl;
                    return 1;
                }
                else
                {
                    if (flags.value("output", QString()).toLower() == "exported_filename")
                    {
                        cout << filePath.toStdString().c_str() << endl;
                    }
                    else
                    {
                        cout << "gta5view-cmd: Exporting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is successful!" << endl;
                    }
                }
            }
            else
            {
                cout << "gta5view-cmd: Reading of " << args.at(1).toStdString().c_str() << " is failed!" << endl;
                return 1;
            }
        }
        else
        {
            SnapmaticPicture picture(":/template.g5e");
            if (picture.readingPicture(true, false, true))
            {
                QImage image;
                QFile imageFile(args.at(1));
                if (!imageFile.open(QFile::ReadOnly))
                {
                    cout << "gta5view-cmd: Reading of " << args.at(1).toStdString().c_str() << " is failed!" << endl;
                    return 1;
                }
                QImageReader imageReader;
                imageReader.setDecideFormatFromContent(true);
                imageReader.setDevice(&imageFile);
                if (!imageReader.read(&image))
                {
                    cout << "gta5view-cmd: Parsing of " << args.at(1).toStdString().c_str() << " is failed!" << endl;
                    return 1;
                }
                if (!image.isNull())
                {
                    QSize snapmaticRes(960, 536);
                    QSize avatarRes(470, 470);
                    int avatarZoneX = 145;
                    int avatarZoneY = 66;

                    if (isDefault)
                    {
                        if (image.width() == image.height())
                        {
                            avatarMode = true;
                        }
                        else
                        {
                            avatarMode = false;
                        }
                    }
                    if (!avatarMode)
                    {
                        QImage snapmaticImage(snapmaticRes, QImage::Format_RGB888);
                        snapmaticImage.fill(Qt::black);
                        QPainter snapmaticPainter(&snapmaticImage);

                        // Picture mode
                        int diffWidth = 0;
                        int diffHeight = 0;
                        if (keepAspectRatio)
                        {
                            image = image.scaled(snapmaticRes, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            if (image.width() != snapmaticRes.width())
                            {
                                diffWidth = snapmaticRes.width() - image.width();
                                diffWidth = diffWidth / 2;
                            }
                            else if (image.height() != snapmaticRes.height())
                            {
                                diffHeight = snapmaticRes.height() - image.height();
                                diffHeight = diffHeight / 2;
                            }
                        }
                        else
                        {
                            image = image.scaled(snapmaticRes.width(), snapmaticRes.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                        }
                        snapmaticPainter.drawImage(0 + diffWidth, 0 + diffHeight, image);
                        snapmaticPainter.end();
                        image = snapmaticImage;
                    }
                    else
                    {
                        QImage snapmaticImage(snapmaticRes, QImage::Format_RGB888);
                        snapmaticImage.fill(Qt::black);
                        QPainter snapmaticPainter(&snapmaticImage);

                        // Scale to Avatar Resolution if needed
                        if (image.width() != avatarRes.width())
                        {
                            image.scaled(avatarRes, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                        }

                        // Avatar mode
                        int diffWidth = 0;
                        int diffHeight = 0;
                        if (keepAspectRatio)
                        {
                            image = image.scaled(avatarRes, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                            if (image.width() > image.height())
                            {
                                diffHeight = avatarRes.height() - image.height();
                                diffHeight = diffHeight / 2;
                            }
                            else if (image.width() < image.height())
                            {
                                diffWidth = avatarRes.width() - image.width();
                                diffWidth = diffWidth / 2;
                            }
                        }
                        else
                        {
                            image = image.scaled(avatarRes, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                        }
                        snapmaticPainter.drawImage(avatarZoneX + diffWidth, avatarZoneY + diffHeight, image);
                        snapmaticPainter.end();
                        image = snapmaticImage;
                    }

                    if (picture.setImage(image))
                    {
                        int crew = 0;
                        QStringList players;
                        if (flags.contains("crew"))
                        {
                            bool crewValid;
                            int crewId = flags["crew"].toInt(&crewValid);
                            if (crewValid)
                            {
                                crew = crewId;
                            }
                        }
                        if (flags.contains("title") && SnapmaticPicture::verifyTitle(flags["title"]))
                        {
                            picture.setPictureTitle(flags["title"]);
                        }
                        else
                        {
                            avatarMode ? picture.setPictureTitle("Converted Avatar") : picture.setPictureTitle("Converted Picture");
                        }
                        if (flags.contains("players"))
                        {
                            bool flagValid = true;
                            QString playerFlag = flags["players"];
                            if (playerFlag.left(1) == "[" && playerFlag.right(1) == "]")
                            {
                                playerFlag.remove(0, 1);
                                playerFlag.remove(playerFlag.length() - 1, 1);
                            }
                            const QStringList playersList = playerFlag.split(",");
                            for (QString player : playersList)
                            {
                                if (player.left(1) == "\"" && player.right(1) == "\"")
                                {
                                    player.remove(0, 1);
                                    player.remove(player.length() - 1, 1);
                                }
                                bool playerValid;
                                player.toInt(&playerValid);
                                if (!playerValid)
                                {
                                    flagValid = false;
                                }
                            }
                            if (flagValid)
                            {
                                players = playersList;
                            }
                        }
                        SnapmaticProperties pictureSP = picture.getSnapmaticProperties();
                        pictureSP.uid = QString(QTime::currentTime().toString("HHmmss") +
                                                QString::number(QDate::currentDate().dayOfYear())).toInt();
                        pictureSP.createdDateTime = QDateTime::currentDateTime();
                        pictureSP.createdTimestamp = pictureSP.createdDateTime.toTime_t();
                        pictureSP.crewID = crew;
                        pictureSP.playersList = players;
                        picture.setSnapmaticProperties(pictureSP);
                        picture.setPictureFileName(QString("PGTA5%1").arg(QString::number(pictureSP.uid)));
                        QString filePath = args.at(2);
                        filePath.replace("<autodef>", picture.getPictureFileName());
                        if (!customFormat)
                        {
                            filePath.replace("<autoext>", "");
                            if (!picture.exportPicture(filePath, SnapmaticFormat::PGTA_Format))
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is failed!" << endl;
                                return 1;
                            }
                            else
                            {
                                if (flags.value("output", QString()).toLower() == "exported_filename")
                                {
                                    cout << filePath.toStdString().c_str() << endl;
                                }
                                else
                                {
                                    cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is successful!" << endl;
                                }
                            }
                        }
                        else
                        {
                            filePath.replace("<autoext>", ".g5e");
                            if (!picture.exportPicture(filePath, SnapmaticFormat::G5E_Format))
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is failed!" << endl;
                                return 1;
                            }
                            else
                            {
                                if (flags.value("output", QString()).toLower() == "exported_filename")
                                {
                                    cout << filePath.toStdString().c_str() << endl;
                                }
                                else
                                {
                                    cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " is successful!" << endl;
                                }
                            }
                        }
                    }
                    else
                    {
                        cout << "gta5view-cmd: Editing of Snapmatic Image Stream failed!" << endl;
                        return 1;
                    }
                }
                else
                {
                    cout << "gta5view-cmd: " << args.at(1).toStdString().c_str() << " is invalid image!" << endl;
                    return 1;
                }
            }
            else
            {
                cout << "gta5view-cmd: Reading of internal template failed!" << endl;
                return 1;
            }
        }
    }
    else
    {
        cout << "gta5view Command Line" << endl << endl;
        cout << "Usage: " << args.at(0).toStdString().c_str() << " source target -format -mode -flag=value" << endl;
        if (args.length() >= 2 && (args.at(1) == "--help" || args.at(1) == "-help"))
        {
            cout << "Convert-only: <autodef> (auto file name at convert)" << endl;
            cout << "Convert-only: <autoext> (auto file extension at convert)" << endl;
            cout << "Formats: jpg pgta g5e" << endl;
            cout << "Modes: a p d aiar akar piar pkar diar dkar" << endl;
            cout << "Flags: crew=int players=int[] title=string" << endl;
        }
        return 255;
    }

    return 0;
}
