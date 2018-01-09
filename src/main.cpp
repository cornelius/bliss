/*
    This file is part of KDE.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

static const char version[] = "0.2";

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("KDE");
  QCoreApplication::setOrganizationDomain("kde.org");
  QCoreApplication::setApplicationName("Bliss");
  QCoreApplication::setApplicationVersion(version);

  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("URL", "Document or directory to open");
  parser.process(app);

  MainWindow mainWin;
  if (!parser.positionalArguments().isEmpty()) {
    mainWin.readData(parser.positionalArguments().first());
  } else {
    mainWin.readData();
  }
  mainWin.show();
  return app.exec();
}
