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

#include "blockstore.h"

#include <QDebug>

BlockStore::BlockStore( QObject *parent )
  : QAbstractListModel( parent )
{
#if 0
  Block *block = new Block;
  block->setTitle( "One" );
  
  mBlocks.append( block );
#endif
}

BlockStore::~BlockStore()
{
  clear();
}

Block::List BlockStore::blocks() const
{
  return mBlocks;
}

void BlockStore::clear()
{
  qDeleteAll( mBlocks );
  mBlocks.clear();
}

int BlockStore::rowCount ( const QModelIndex & ) const
{
  return mBlocks.size();
}

QVariant BlockStore::data ( const QModelIndex & index, int role ) const
{
  if (!index.isValid()) return QVariant();

  if (index.row() >= mBlocks.size()) return QVariant();

  if (role == Qt::DisplayRole) return mBlocks[ index.row() ]->title();

  return QVariant();
}

void BlockStore::add( Block *block )
{
  beginInsertRows( QModelIndex(), mBlocks.size(), mBlocks.size() );

  mBlocks.append( block );

  endInsertRows();
}

Block *BlockStore::block( const QModelIndex &index ) const
{
  int blockIndex = index.row();
  if ( blockIndex < 0 || blockIndex >= mBlocks.size() ) {
    qWarning() << "Block index out of range.";
    return new Block();
  }

  return mBlocks[ blockIndex ];
}

void BlockStore::updateBlock( const QModelIndex &index )
{
  emit dataChanged( index, index );
}
