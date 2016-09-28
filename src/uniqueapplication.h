/***
  This file is part of pavucontrol-qt.

  Copyright 2006-2008 Lennart Poettering
  Copyright 2008 Sjoerd Simons <sjoerd@luon.net>
  Copyright 2016 Helio Chissini de Castro <helio@kde.org>

  pavucontrol is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  pavucontrol is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with pavucontrol. If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef UNIQUEAPPLICATION_H
#define UNIQUEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>

class UniqueApplication : public QApplication
{
    Q_OBJECT

public:
    UniqueApplication(int &argc, char** argv);
    ~UniqueApplication();

    bool active();

private:
    QSharedMemory *uniq;
};

#endif // UNIQUEAPPLICATION_H
