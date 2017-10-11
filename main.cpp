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
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if (args.length() >= 3)
    {
        bool convertToGTA = true;
        bool customFormat = false;
        if (args.length() == 4)
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
                    if (image.size() != snapmaticRes) image = image.scaled(snapmaticRes, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    if (picture.setImage(image))
                    {
                        picture.setPictureTitle("Converted Picture");
                        SnapmaticProperties pictureSP = picture.getSnapmaticProperties();
                        pictureSP.uid = QString(QTime::currentTime().toString("HHmmss") +
                                                QString::number(QDate::currentDate().dayOfYear())).toInt();
                        pictureSP.createdDateTime = QDateTime::currentDateTime();
                        pictureSP.createdTimestamp = pictureSP.createdDateTime.toTime_t();
                        picture.setSnapmaticProperties(pictureSP);
                        picture.setPicFileName(QString("PGTA5%1").arg(QString::number(pictureSP.uid)));
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
        cout << "Usage: " << args.at(0).toStdString().c_str() << " source target -format" << endl;
        if (args.at(1) == "--help")
        {
            cout << "Convert-only: <autodef> (auto file name at convert)" << endl;
            cout << "Convert-only: <autoext> (auto file extension at convert)" << endl;
            cout << "Formats: jpg pgta g5e" << endl;
        }
        return 255;
    }

    return 0;
}

