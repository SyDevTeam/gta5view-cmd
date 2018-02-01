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
#include <QDateTime>
#include <QPainter>
#include <QImage>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if (args.length() >= 3)
    {
        bool isDefault = true;
        bool avatarMode = false;
        bool convertToGTA = true;
        bool customFormat = false;
        bool keepAspectRatio = true;
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
                    cout << "gta5view-cmd: Exporting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " failed!" << endl;
                    return 1;
                }
            }
            else
            {
                cout << "gta5view-cmd: Reading of " << args.at(1).toStdString().c_str() << " failed!" << endl;
                return 1;
            }
        }
        else
        {
            SnapmaticPicture picture(":/template.g5e");
            if (picture.readingPicture(true, false, true))
            {
                QImage image(args.at(1));
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
                        avatarMode ? picture.setPictureTitle("Converted Avatar") : picture.setPictureTitle("Converted Picture");
                        SnapmaticProperties pictureSP = picture.getSnapmaticProperties();
                        pictureSP.uid = QString(QTime::currentTime().toString("HHmmss") +
                                                QString::number(QDate::currentDate().dayOfYear())).toInt();
                        pictureSP.createdDateTime = QDateTime::currentDateTime();
                        pictureSP.createdTimestamp = pictureSP.createdDateTime.toTime_t();
                        picture.setSnapmaticProperties(pictureSP);
                        picture.setPictureFileName(QString("PGTA5%1").arg(QString::number(pictureSP.uid)));
                        QString filePath = args.at(2);
                        filePath.replace("<autodef>", picture.getPictureFileName());
                        if (!customFormat)
                        {
                            filePath.replace("<autoext>", "");
                            if (!picture.exportPicture(filePath, SnapmaticFormat::PGTA_Format))
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " failed!" << endl;
                                return 1;
                            }
                            else
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " successful!" << endl;
                            }
                        }
                        else
                        {
                            filePath.replace("<autoext>", ".g5e");
                            if (!picture.exportPicture(filePath, SnapmaticFormat::G5E_Format))
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " failed!" << endl;
                                return 1;
                            }
                            else
                            {
                                cout << "gta5view-cmd: Converting of " << args.at(1).toStdString().c_str() << " to " << filePath.toStdString().c_str() << " successful!" << endl;
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
                    cout << "gta5view-cmd: Reading of " << args.at(1).toStdString().c_str() << " failed!" << endl;
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
        cout << "Usage: " << args.at(0).toStdString().c_str() << " source target -format -mode" << endl;
        if (args.length() >= 1 && args.at(1) == "--help")
        {
            cout << "Convert-only: <autodef> (auto file name at convert)" << endl;
            cout << "Convert-only: <autoext> (auto file extension at convert)" << endl;
            cout << "Formats: jpg pgta g5e" << endl;
            cout << "Modes: a p d aiar akar piar pkar diar dkar" << endl;
        }
        return 255;
    }

    return 0;
}

