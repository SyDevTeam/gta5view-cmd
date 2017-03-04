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
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if (args.length() == 3)
    {
        SnapmaticPicture picture(args.at(1));
        if (picture.readingPicture(true, false, true))
        {
            if (!picture.exportPicture(args.at(2), "JPG"))
            {
                cout << "gta5view-cmd: Exporting of " << args.at(1).toStdString().c_str() << " to " << args.at(2).toStdString().c_str() << " failed!" << endl;
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
        cout << "Usage: " << args.at(0).toStdString().c_str() << " source target" << endl;
        return 255;
    }

	return 0;
}

