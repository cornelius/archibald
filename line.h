/*
    This file is part of KDE.

    Copyright (c) 2008 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef LINE_H
#define LINE_H

class Line
{
  public:
    Line();

    void setEnabled( bool e ) { m_enabled = e; }
    bool isEnabled() const { return m_enabled; }

    void setFrom( int from ) { m_from = from; }
    int from() const { return m_from; }
    
    void setTo( int to ) { m_to = to; }
    int to() const { return m_to; }
    
    void setStyle( int style ) { m_style = style; }
    int style() const { return m_style; }
    
  private:
    bool m_enabled;
    int m_from;
    int m_to;
    int m_style;
};

#endif
