/*
    This file is part of KDE.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "ringwidget.h"

#include "blockstore.h"

#include <QPainter>
#include <QDebug>
#include <QPainterPath>

#include <math.h>

RingWidget::RingWidget( QWidget *parent )
  : QFrame( parent ), mBlockStore( 0 ), mPainter( 0 )
{
  setBackgroundRole( QPalette::Base );
  setAutoFillBackground( true );
  setContentsMargins( 8, 8, 8, 8 );
}

QSize RingWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RingWidget::sizeHint() const
{
    return QSize(200, 200);
}

void RingWidget::paintEvent( QPaintEvent * )
{
  if ( !mBlockStore ) return;

  mPainter = new QPainter( this );
  mPainter->setRenderHint( QPainter::Antialiasing );

  mPen = mPainter->pen();

  QRect r = contentsRect();

  mCenter = QPointF( r.center() );

  int count = mBlockStore->blocks().size();

  Q_UNUSED( count )

  int maxRing = 0;

  foreach( Block *block, mBlockStore->blocks() ) {
    if ( block->ring() > maxRing ) maxRing = block->ring();
  }

  foreach( Block *block, mBlockStore->blocks() ) {
    mPen.setWidth( 2 );
    mPen.setColor( block->color() );
    mPainter->setPen( mPen );

    qreal radius = qMin( r.width() / 2, r.height() / 2 );
    qreal ringHeight = radius / maxRing;
    qreal blockHeight = ringHeight * 3 / 4;
    qreal blockRadius = ringHeight * block->ring();

    QRectF outerBlockRect(
      mCenter.x() - blockRadius,
      mCenter.y() - blockRadius,
      ( blockRadius ) * 2,
      ( blockRadius ) * 2 );
    QRectF innerBlockRect(
      mCenter.x() - blockRadius + blockHeight,
      mCenter.y() - blockRadius + blockHeight,
      ( blockRadius - blockHeight ) * 2,
      ( blockRadius - blockHeight ) * 2 );

//      painter.drawRect( outerBlockRect );
//      painter.drawRect( innerBlockRect );

    int startAngle = 360 - block->startAngle() - 90;
    int endAngle = 360 - block->endAngle() - 90;

    QPainterPath blockPath;

    if ( block->circle() ) {
      blockPath.addEllipse( outerBlockRect );
    } else {
      QPainterPath helperPath;

      helperPath.arcTo( outerBlockRect, startAngle, 0 );
      QPointF upperLeftPos = helperPath.currentPosition();

      blockPath.moveTo( upperLeftPos );
      blockPath.arcTo( outerBlockRect, startAngle, endAngle - startAngle );

      if ( block->pie() ) {
        blockPath.lineTo( mCenter );
      } else {
        helperPath.arcTo( innerBlockRect, endAngle, 0 );
        QPointF lowerRightPos = helperPath.currentPosition();

        blockPath.lineTo( lowerRightPos );
        blockPath.arcTo( innerBlockRect, endAngle, startAngle - endAngle );
      }

      blockPath.closeSubpath();
    }
    
    mPainter->drawPath( blockPath );

    if ( block->line().isEnabled() ) {
      QPainterPath helperPath;

      int lineAngle = ( endAngle - startAngle ) / 2;

      helperPath.arcTo( innerBlockRect, startAngle, lineAngle );
      
      QPointF startPos = helperPath.currentPosition();

      qreal destinationBlockRadius = ringHeight * block->line().to();

      int blockOffset = 1;

      QRectF destinationBlockRect(
        mCenter.x() - destinationBlockRadius - blockOffset,
        mCenter.y() - destinationBlockRadius - blockOffset,
        ( destinationBlockRadius ) * 2 + blockOffset * 2,
        ( destinationBlockRadius ) * 2 + blockOffset * 2 );

      helperPath.arcTo( destinationBlockRect, startAngle, lineAngle );

      QPointF endPos = helperPath.currentPosition();
      
      QPen pen( mPen );
      pen.setStyle( Qt::PenStyle( block->line().style() + 1 ) );
      mPainter->setPen( pen );
      
      mPainter->drawLine( startPos, endPos );
    }

    if ( block->secondaryLine().isEnabled() ) {
      QPainterPath helperPath;
      int blockOffset = 1;

      int lineAngle = ( endAngle - startAngle ) / 2;


      qreal fromBlockRadius = ringHeight * ( block->secondaryLine().from() );

      QRectF fromBlockRect(
        mCenter.x() - fromBlockRadius + blockHeight,
        mCenter.y() - fromBlockRadius + blockHeight,
        ( fromBlockRadius - blockHeight ) * 2,
        ( fromBlockRadius - blockHeight ) * 2 );

      helperPath.arcTo( fromBlockRect, startAngle, lineAngle );
      
      QPointF startPos = helperPath.currentPosition();


      qreal destinationBlockRadius = ringHeight * block->secondaryLine().to();

      QRectF destinationBlockRect(
        mCenter.x() - destinationBlockRadius - blockOffset,
        mCenter.y() - destinationBlockRadius - blockOffset,
        ( destinationBlockRadius ) * 2 + blockOffset * 2,
        ( destinationBlockRadius ) * 2 + blockOffset * 2 );

      helperPath.arcTo( destinationBlockRect, startAngle, lineAngle );


      QPointF endPos = helperPath.currentPosition();
      
      QPen pen( mPen );
      pen.setStyle( Qt::PenStyle( block->secondaryLine().style() + 1 ) );
      mPainter->setPen( pen );
      
      mPainter->drawLine( startPos, endPos );
    }

    if ( !block->hideText() ) {
  //    qDebug() << "TEXT: " << block->title() << "  ANGLE: " << startAngle;

      QPainterPath textPath;

      int fontHeight = int( blockHeight / 2 );
      QFont font( "Helvetica", fontHeight );
      qreal textRadius = blockRadius - blockHeight +
        ( blockHeight - fontHeight ) / 2;
      textPath.addText( mCenter.x(), mCenter.y() - textRadius, font,
        block->title() );

      mTextScale = 360 / ( textRadius * 2 * M_PI );

      int angle;
      if ( block->circle() ) angle = 130;
      else angle = startAngle;

      textPath = bendPath( textPath, angle );

      QPen pen = mPen;
      pen.setWidth( 1 );
      mPainter->setPen( pen );
      mPainter->fillPath( textPath, QBrush( pen.color() ) );
      mPainter->setPen( mPen );
    }
  }

#if 0
  QPainterPath path;

#if 0
  path.moveTo( 0, 0 );
  path.lineTo( r.width(), r.height() );
#endif

  QFont font( "Helvetica", 30 );
  path.addText( mCenter.x(), 100, font, "Hallo" );

#if 0
  mPen.setColor( "black" );
  mPen.setWidth( 1 );
  mPainter->setPen( mPen );
  mPainter->drawPath( path );
#endif

//  dumpPath( path );

  QPainterPath bendedPath = bendPath( path );

//  dumpPath( bendedPath );

  mPen.setColor( "green" );
  mPainter->setPen( mPen );
  mPainter->drawPath( bendedPath );

//  painter.drawEllipse( r );
#endif

  delete mPainter;
  mPainter = 0;
}

void RingWidget::dumpPath( const QPainterPath &path )
{
  qDebug() << "PATH";
  for (int i=0; i<path.elementCount(); ++i) {
    QPainterPath::Element e = path.elementAt( i );
    qDebug() << "X: " << e.x << "  Y: " << e.y << " TYPE: " << e.type;
  }
}

QPainterPath RingWidget::bendPath( const QPainterPath &path, int angle )
{
  QPainterPath result;

  QPointF point1, point2;

  bool curveData = false;

  qreal offsetAngle = ( angle - 90 ) * 2 * M_PI / 360;

//  qDebug() << "ANGLE: " << angle << "  RADIAN: " << offsetAngle;

  for (int i=0; i<path.elementCount(); ++i) {
    QPainterPath::Element e = path.elementAt( i );

    QPointF point = transform ( e.x, e.y, offsetAngle );
    
    if ( e.type == 0 ) {
      result.moveTo( point );
    } else if ( e.type == 1 ) {
      result.lineTo( point );
    } else if ( e.type == 2 ) {
      point1 = point;
      curveData = false;
    } else if ( e.type == 3 ) {
      if ( !curveData ) {
        curveData = true;
        point2 = point;
      } else {
        result.cubicTo( point1, point2, point );
      }
    }
  }
  
  return result;
}

QPointF RingWidget::transform( qreal xIn, qreal yIn, qreal a )
{
#if 0
  qDebug() << "TRANSFORM";
  
  qDebug() << "  XIN: " << xIn << "  YIN" << yIn;
#endif

  qreal radius = mCenter.y() - yIn;
  
  qreal angle = ( xIn - mCenter.x() + 5 ) * mTextScale * 2 * M_PI / 360 - a;

#if 0
  qDebug() << "  ANGLE: " << angle << "  RADIUS: " << radius;

  qDebug() << "  SIN: " << sin( angle ) << "  COS: " << cos( angle );
#endif

  qreal xOut = mCenter.x() + radius * sin( angle );
  qreal yOut = mCenter.y() - radius * cos( angle );

#if 0
  qDebug() << "  XOUT: " << xOut << "  YOUT: " << yOut;
#endif

  return QPointF( xOut, yOut );
}

void RingWidget::setModel( BlockStore *store )
{
  mBlockStore = store;

  connect( mBlockStore,
    SIGNAL( dataChanged ( const QModelIndex &, const QModelIndex & ) ),
    SLOT( updateWidget() ) );
  connect( mBlockStore,
    SIGNAL( rowsInserted ( const QModelIndex &, int, int ) ),
    SLOT( updateWidget() ) );
}

void RingWidget::updateWidget()
{
  update();
}
