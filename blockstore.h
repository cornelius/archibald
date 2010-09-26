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
#ifndef BLOCKSTORE_H
#define BLOCKSTORE_H

#include <QAbstractListModel>

#include "block.h"

class BlockStore : public QAbstractListModel
{
  public:
    BlockStore( QObject *parent = 0 );
    ~BlockStore();

    void setTitle( const QString &title );
    QString title() const;

    void clear();

    Block::List blocks() const;

    Block *block( const QModelIndex & ) const;

    void updateBlock( const QModelIndex & );

    void add( Block * );

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index,
      int role = Qt::DisplayRole ) const;

  private:
    QString mTitle;
    Block::List mBlocks;
};

#endif
