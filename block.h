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
#ifndef BLOCK_H
#define BLOCK_H

#include "line.h"

#include <QString>
#include <QList>
#include <QColor>

class Block
{
  public:
    typedef QList<Block *> List;
  
    Block();

    void setTitle( const QString &title ) { mTitle = title; }
    QString title() const { return mTitle; }

    void setRing( int i ) { mRing = i; }
    int ring() const { return mRing; }

    void setStartAngle( int i ) { mStartAngle = i; }
    int startAngle() const { return mStartAngle; }

    void setEndAngle( int i ) { mEndAngle = i; }
    int endAngle() const { return mEndAngle; }

    void setColor( const QColor &c ) { mColor = c; }
    QColor color() const { return mColor; }

    void setHideText( bool b ) { mHideText = b; }
    bool hideText() const { return mHideText; }
    
    void setPie( bool b ) { mPie = b; }
    bool pie() const { return mPie; }
    
    void setCircle( bool b ) { mCircle = b; }
    bool circle() const { return mCircle; }

    void setLine( const Line &l ) { mLine = l; }
    Line line() const { return mLine; }
    
    void setSecondaryLine( const Line &l ) { mSecondaryLine = l; }
    Line secondaryLine() const { return mSecondaryLine; }

  private:
    QString mTitle;
    int mRing;
    int mStartAngle;
    int mEndAngle;
    QColor mColor;
    bool mHideText;
    bool mPie;
    bool mCircle;
    Line mLine;
    Line mSecondaryLine;
};

#endif
