/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>
#include <QMouseEvent>

#include "knmusicratingeditor.h"

KNMusicRatingEditor::KNMusicRatingEditor(QWidget *parent) :
    QWidget(parent),
    m_star(QPixmap(":/plugin/music/public/star.png")),
    m_unstar(QPixmap(":/plugin/music/public/unstar.png")),
    m_scaleStar(QPixmap()),
    m_scaleUnstar(QPixmap()),
    m_starSizeHint(0),
    m_editorWidth(0),
    m_starNum(0),
    m_halfStarSizeHint(0),
    m_detectOnMove(true)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setMouseTracking(true);
    setAutoFillBackground(true);

    //Set default background to black.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0));
    setPalette(pal);
}

void KNMusicRatingEditor::paintEvent(QPaintEvent *event)
{
    //Check out the star pixmap.
    if(m_scaleStar.isNull())
    {
        //Ignore the null painting event.
        return;
    }
    //Do the original paint.
    QWidget::paintEvent(event);
    //Set the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Set the pen and brush.
    painter.setPen(Qt::NoPen);
    painter.fillRect(rect(), palette().color(QPalette::Window));
    //Paint the star number.
    for(int i=0; i<m_starNum; i++)
    {
        painter.drawPixmap(i*m_starSizeHint,
                           0,
                           m_scaleStar);
    }
    for(int i=m_starNum; i<5; i++)
    {
        painter.drawPixmap(i*m_starSizeHint,
                           0,
                           m_scaleUnstar);
    }
}

void KNMusicRatingEditor::mousePressEvent(QMouseEvent *event)
{
    //Check the detecting switcher.
    if(!m_detectOnMove)
    {
        //When the mouse moving detecting is disabled, mouse press will be the
        //most important things.
        int star=starAtPosition(event->x());
        //If the star number is valid, change the star number.
        if(star!=-1)
        {
            //Change the star number.
            setStarNum(star);
            //Repaint the widget.
            update();
        }
    }
}

void KNMusicRatingEditor::mouseMoveEvent(QMouseEvent *event)
{
    //Check the detecting switcher.
    if(m_detectOnMove)
    {
        //When the mouse is moving, change the value.
        int star=starAtPosition(event->x());
        //If the star number is valid, change the star number.
        if(star!=-1)
        {
            //Change the star number.
            setStarNum(star);
            //Repaint the widget.
            update();
        }
    }
}

void KNMusicRatingEditor::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original release event.
    QWidget::mouseReleaseEvent(event);
    //When the mouse released, means the editing is complete.
    emit editingFinished();
}

inline int KNMusicRatingEditor::starAtPosition(int x)
{
    //If mouse's x is less than a half star, it should be 0.
    int starNum=x<m_halfStarSizeHint?0:(x/m_starSizeHint+1);
    if(starNum<0)
    {
        return -1;
    }
    return starNum>5?5:starNum;
}

void KNMusicRatingEditor::setDetectOnMove(bool detectOnMove)
{
    m_detectOnMove = detectOnMove;
}

int KNMusicRatingEditor::starNum() const
{
    return m_starNum;
}

void KNMusicRatingEditor::setStarNum(int starNum)
{
    //Save the star number.
    m_starNum = starNum;
    //Update the widget.
    update();
}

int KNMusicRatingEditor::starSizeHint() const
{
    return m_starSizeHint;
}

void KNMusicRatingEditor::setStarSizeHint(int starSizeHint)
{
    //Get the star size.
    m_starSizeHint=starSizeHint;
    //Width of editor should be five stars.
    m_editorWidth=m_starSizeHint*5;
    //Half size of star.
    m_halfStarSizeHint=m_starSizeHint>>1;
    //Scale the pixmap to the size.
    m_scaleStar=m_star.scaled(m_starSizeHint,
                              m_starSizeHint,
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation);
    m_scaleUnstar=m_unstar.scaled(m_starSizeHint,
                                  m_starSizeHint,
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
}
