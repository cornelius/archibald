/*
    This file is part of KDE.

    Copyright (c) 2006,2010 Cornelius Schumacher <schumacher@kde.org>

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
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "storage.h"

#include "model.h"

#include <Qt>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QDebug>
#include <QRegExp>
#include <Q3StyleSheet>

Storage::Storage( Model *model )
  : mModel( model )
{
}

void Storage::load( const QString &filename )
{
//  qDebug() << "Storage::load()";

  QFile file( filename );
  if ( !file.open( QFile::ReadOnly ) ) {
    QMessageBox::critical( 0, "Load Error", "Unable to load file" );
  } else {
    QDomDocument doc;
    doc.setContent( &file );
    
    QDomElement element = doc.documentElement();

    QDomNode n;
    for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
      QDomElement e = n.toElement();
      if ( e.tagName() == "diagram" ) {
        parseDiagram( e );
      } else {
        qDebug() << "Unknown tag: " << e.tagName();
      }
    }
  }
}

void Storage::parseDiagram( const QDomElement &element )
{
  BlockStore *store = new BlockStore;

  store->setTitle( element.attribute( "title" ) );

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "block" ) {
      store->add( parseBlock( e ) );
    } else {
      qDebug() << "Unknown tag: " << e.tagName();
    }
  }
  
  mModel->addBlockStore( store );
}

Block *Storage::parseBlock( const QDomElement &element )
{
  Block *b = new Block;

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "title" ) {
      b->setTitle( e.text() );
    } else if ( e.tagName() == "ring" ) {
      b->setRing( e.text().toInt() );
    } else if ( e.tagName() == "startangle" ) {
      b->setStartAngle( e.text().toInt() );
    } else if ( e.tagName() == "endangle" ) {
      b->setEndAngle( e.text().toInt() );
    } else if ( e.tagName() == "color" ) {
      b->setColor( e.text() );
    } else if ( e.tagName() == "hidetext" ) {
      if ( e.text() == "true" ) b->setHideText( true );
      else b->setHideText( false );
    } else if ( e.tagName() == "pie" ) {
      if ( e.text() == "true" ) b->setPie( true );
      else b->setPie( false );
    } else if ( e.tagName() == "circle" ) {
      if ( e.text() == "true" ) b->setCircle( true );
      else b->setCircle( false );
    } else if ( e.tagName() == "color" ) {
      b->setColor( e.text() );
    } else if ( e.tagName() == "line" ) {
      Line line;
      line.setEnabled( true );
      line.setTo( e.attribute( "destination" ).toInt() );
      line.setStyle( e.attribute( "style" ).toInt() );
      b->setLine( line );
    } else if ( e.tagName() == "secondaryline" ) {
      Line line;
      line.setEnabled( true );
      line.setFrom( e.attribute( "from" ).toInt() );
      line.setTo( e.attribute( "destination" ).toInt() );
      line.setStyle( e.attribute( "style" ).toInt() );
      b->setSecondaryLine( line );
    } else {
      qDebug() << "Unknown tag: " << e.tagName();
    }
  }

  return b;
}

void Storage::save( const QString &filename )
{
  QFile file( filename );
  if ( !file.open( QFile::WriteOnly ) ) {
    QMessageBox::critical( 0, "Save Error", "Unable to save file" );
  } else {
    QTextStream ts( &file );
    ts << "<archibald>\n";
    foreach( BlockStore *blockStore, mModel->blockStores() ) {
      ts << "  <diagram title=\"" << blockStore->title() << "\">\n";
      Block::List blocks = blockStore->blocks();
      foreach( Block *b, blocks ) {
        ts << "    <block>\n";
        ts << "      <title>" + Q3StyleSheet::escape( b->title() ) + "</title>\n";
        ts << "      <ring>" + QString::number( b->ring() ) + "</ring>\n";
        ts << "      <startangle>" + QString::number( b->startAngle() ) +
          "</startangle>\n";
        ts << "      <endangle>" + QString::number( b->endAngle() ) +
          "</endangle>\n";
        ts << "      <color>" + b->color().name() + "</color>\n";

        ts << "      <hidetext>";
        if ( b->hideText() ) ts << "true";
        else ts << "false";
        ts << "</hidetext>\n";

        ts << "      <pie>";
        if ( b->pie() ) ts << "true";
        else ts << "false";
        ts << "</pie>\n";

        ts << "      <circle>";
        if ( b->circle() ) ts << "true";
        else ts << "false";
        ts << "</circle>\n";

        if ( b->line().isEnabled() ) {
          ts << "      <line";
          ts << " destination=\"" << b->line().to() << "\"";
          ts << " style=\"" << b->line().style() << "\"";
          ts << "/>\n";
        }

        if ( b->secondaryLine().isEnabled() ) {
          ts << "      <secondaryline";
          ts << " destination=\"" << b->secondaryLine().to() << "\"";
          ts << " from=\"" << b->secondaryLine().from() << "\"";
          ts << " style=\"" << b->secondaryLine().style() << "\"";
          ts << "/>\n";
        }

        ts << "    </block>\n";
      }
      ts << "  </diagram>\n";
    }
    ts << "</archibald>\n";
  }
}
