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
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef STORAGE_H
#define STORAGE_H

#include "block.h"

#include <QString>
#include <QDomElement>

class Model;

class Storage
{
  public:
    Storage( Model * );

    void load( const QString & filename );
    void save( const QString & filename );
  
  protected:
    void parseDiagram( const QDomElement &element );
    Block *parseBlock( const QDomElement &element );
  
  private:
    Model *mModel;
};

#endif
