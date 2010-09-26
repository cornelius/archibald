#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "model.h"

#include <QMainWindow>
#include <ui_mainwindow.h>

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT
  public:
    MainWindow();
    ~MainWindow();

  public slots:
    void showRingView();
    void save();
    void load();

  protected:
    void loadBlockEditor( Block * );
    void saveBlockEditor( Block * );

    QString defaultFilename() const;

    void setColor( const QColor & );
    QColor blockColor() const;

  protected slots:
    void addBlock();
    void saveBlockEditor();

    void slotCurrentChanged( const QModelIndex &current,
      const QModelIndex &previous );

    void selectColor();

  private:
    Model mModel;
    QModelIndex mCurrentBlockIndex;
    
    bool mLoading;
};

#endif
