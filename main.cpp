#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  MainWindow mainWindow;
  mainWindow.show();
    
//  mainWindow.showRingView();
    
  return app.exec();
}
