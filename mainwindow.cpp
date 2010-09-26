#include "mainwindow.h"

#include "ringwidget.h"
#include "storage.h"
#include "prefs.h"

#include <QtDebug>
#include <QColorDialog>

MainWindow::MainWindow()
  : mLoading( false )
{
  setupUi( this );
  
  connect( mActionQuit, SIGNAL( triggered() ), SLOT( close() ) );

  connect( mStartAngleSpin, SIGNAL( valueChanged( int ) ),
    mStartAngleDial, SLOT( setValue( int ) ) );
  connect( mStartAngleDial, SIGNAL( valueChanged( int ) ),
    mStartAngleSpin, SLOT( setValue( int ) ) );

  connect( mEndAngleSpin, SIGNAL( valueChanged( int ) ),
    mEndAngleDial, SLOT( setValue( int ) ) );
  connect( mEndAngleDial, SIGNAL( valueChanged( int ) ),
    mEndAngleSpin, SLOT( setValue( int ) ) );

  connect( mRingViewButton, SIGNAL( clicked() ),
    SLOT( showRingView() ) );

  mBlockListView->setModel( &mBlockStore );
  mView->setModel( &mBlockStore );

  connect( mAddBlockButton, SIGNAL( clicked() ), SLOT( addBlock() ) );

  connect( mBlockListView->selectionModel(),
    SIGNAL( currentChanged ( const QModelIndex &, const QModelIndex & ) ),
    SLOT( slotCurrentChanged( const QModelIndex &, const QModelIndex & ) ) );

  load();

  mBlockListView->selectionModel()->setCurrentIndex( mBlockStore.index( 0 ),
    QItemSelectionModel::SelectCurrent );

  connect( mTitleEdit, SIGNAL( textEdited( const QString & ) ),
    SLOT( saveBlockEditor() ) );
  connect( mRingSpin, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );

  connect( mStartAngleSpin, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mEndAngleSpin, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );

  connect( mColorSelectButton, SIGNAL( clicked() ),
    SLOT( selectColor() ) );

  connect( mHideTextCheck, SIGNAL( stateChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mPieCheck, SIGNAL( stateChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mCircleCheck, SIGNAL( stateChanged( int ) ),
    SLOT( saveBlockEditor() ) );

  connect( mLineCheck, SIGNAL( toggled( bool ) ),
    SLOT( saveBlockEditor() ) );
  connect( mLineDestinationSpin, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mLineStyleCombo, SIGNAL( currentIndexChanged( int ) ),
    SLOT( saveBlockEditor() ) );

  connect( mLineCheck_2, SIGNAL( toggled( bool ) ),
    SLOT( saveBlockEditor() ) );
  connect( mSecondaryLineFromSpin, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mLineDestinationSpin_2, SIGNAL( valueChanged( int ) ),
    SLOT( saveBlockEditor() ) );
  connect( mLineStyleCombo_2, SIGNAL( currentIndexChanged( int ) ),
    SLOT( saveBlockEditor() ) );

  QRect g = Prefs::self()->mainWindowGeometry();
  if ( g.isValid() ) setGeometry( g );

  setCaption( "Archibald" );
}

MainWindow::~MainWindow()
{
  save();

  Prefs::self()->setMainWindowGeometry( geometry() );

  Prefs::self()->writeConfig();

  delete Prefs::self();
}

void MainWindow::showRingView()
{
  RingWidget *ringWidget = new RingWidget( 0 );
  ringWidget->setModel( &mBlockStore );
  ringWidget->show();
}

void MainWindow::addBlock()
{
  qDebug() << "MainWindow::addBlock()";
  
  mBlockStore.add( new Block() );

  mBlockListView->selectionModel()->setCurrentIndex(
    mBlockStore.index( mBlockStore.blocks().size() - 1 ),
    QItemSelectionModel::SelectCurrent );
}

void MainWindow::slotCurrentChanged( const QModelIndex &current,
  const QModelIndex &previous )
{
  Q_UNUSED( previous )
  
  mCurrentBlockIndex = current;

  loadBlockEditor( mBlockStore.block( mCurrentBlockIndex ) );
}

void MainWindow::loadBlockEditor( Block *block )
{
#if 0
  qDebug() << "Start angle: " << block->startAngle();
  qDebug() << "End angle: " << block->endAngle();
#endif

  mLoading = true;

  mTitleEdit->setText( block->title() );
  mRingSpin->setValue( block->ring() );
  mStartAngleDial->setValue( block->startAngle() );
  mEndAngleDial->setValue( block->endAngle() );
  mStartAngleSpin->setValue( block->startAngle() );
  mEndAngleSpin->setValue( block->endAngle() );
  mHideTextCheck->setChecked( block->hideText() );
  mPieCheck->setChecked( block->pie() );
  mCircleCheck->setChecked( block->circle() );

  setColor( block->color() );

  if ( block->line().isEnabled() ) {
    mLineCheck->setChecked( true );
    mLineDestinationSpin->setValue( block->line().to() );
    mLineStyleCombo->setCurrentIndex( block->line().style() );
  } else {
    mLineCheck->setChecked( false );
  }

  if ( block->secondaryLine().isEnabled() ) {
    mLineCheck_2->setChecked( true );
    mSecondaryLineFromSpin->setValue( block->secondaryLine().from() );
    mLineDestinationSpin_2->setValue( block->secondaryLine().to() );
    mLineStyleCombo_2->setCurrentIndex( block->secondaryLine().style() );
  } else {
    mLineCheck_2->setChecked( false );
  }

  mLoading = false;
}

void MainWindow::saveBlockEditor()
{
  if ( mCurrentBlockIndex.isValid() ) {
    saveBlockEditor( mBlockStore.block( mCurrentBlockIndex ) );
    mBlockStore.updateBlock( mCurrentBlockIndex );
  }
}

void MainWindow::saveBlockEditor( Block *block )
{
  if ( mLoading ) return;

//  qDebug() << "MainWindow::saveBlockEditor()";

  block->setTitle( mTitleEdit->text() );
  block->setRing( mRingSpin->value() );
  block->setStartAngle( mStartAngleSpin->value() );
  block->setEndAngle( mEndAngleSpin->value() );
  block->setColor( blockColor() );
  block->setHideText( mHideTextCheck->isChecked() );
  block->setPie( mPieCheck->isChecked() );
  block->setCircle( mCircleCheck->isChecked() );
  if ( mLineCheck->isChecked() ) {
    Line line;
    line.setEnabled( true );
    line.setTo( mLineDestinationSpin->value() );
    line.setStyle( mLineStyleCombo->currentIndex() );
    block->setLine( line );
  } else {
    block->setLine( Line() );
  }
  if ( mLineCheck_2->isChecked() ) {
    Line line;
    line.setEnabled( true );
    line.setFrom( mSecondaryLineFromSpin->value() );
    line.setTo( mLineDestinationSpin_2->value() );
    line.setStyle( mLineStyleCombo_2->currentIndex() );
    block->setSecondaryLine( line );
  } else {
    block->setSecondaryLine( Line() );
  }
}

void MainWindow::save()
{
//  qDebug() << "MainWindow::save()";

  Storage s( &mBlockStore );
  s.save( defaultFilename() );
}

void MainWindow::load()
{
  Storage s( &mBlockStore );
  s.load( defaultFilename() );
}

QString MainWindow::defaultFilename() const
{
  return "archibald.xml";
}

void MainWindow::selectColor()
{
  QColor newColor = QColorDialog::getColor( blockColor(), this );
  
  setColor( newColor );
}

void MainWindow::setColor( const QColor &color )
{
  QPalette palette = mColorFrame->palette();
  palette.setColor( QPalette::Window, color );
  mColorFrame->setPalette( palette );
  saveBlockEditor();
}

QColor MainWindow::blockColor() const
{
  return mColorFrame->palette().color( QPalette::Window );
}
