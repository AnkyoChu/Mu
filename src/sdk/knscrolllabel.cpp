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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimer>
#include <QPainter>
#include <QSizePolicy>

#include "knscrolllabel.h"

#include <QDebug>

#define ShortWaiting 2500
#define LongWaiting 3000

KNScrollLabel::KNScrollLabel(QWidget *parent) :
    QWidget(parent),
    m_text(QString()),
    m_move(new QTimer(this)),
    m_wait(new QTimer(this)),
    m_opacity(1.0),
    m_glowRadius(9.0),
    m_textLeftMostX(0),
    m_textX(0),
    m_movingLeft(true)
{
    //Set properties.
    setContentsMargins(m_glowRadius/2,
                       m_glowRadius/2,
                       m_glowRadius/2,
                       m_glowRadius/2);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                                  QSizePolicy::Minimum,
                                  QSizePolicy::Label));

    //Configure the move and waiting timer.
    m_move->setInterval(33);
    connect(m_move, &QTimer::timeout, this, &KNScrollLabel::moveText);
    m_wait->setInterval(LongWaiting);
    m_wait->setSingleShot(true);
    connect(m_wait, &QTimer::timeout, [=]{m_move->start();});
}

void KNScrollLabel::paintEvent(QPaintEvent *event)
{
    //Ignore the previous event.
    Q_UNUSED(event);
    //If the text is empty, then draw nothing.
    if(m_text.isEmpty())
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    //Set the rendering hints.
    painter.setRenderHints(QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Configure the font.
    painter.setFont(font());
    //Configure the opacity.
    painter.setOpacity(m_opacity);
    //Configure the pen.
    painter.setPen(palette().color(QPalette::WindowText));
    //Draw content text.
    painter.drawText(m_textX,
                     contentsRect().y(),
                     fontMetrics().width(m_text)+m_glowRadius,
                     height(),
                     Qt::AlignLeft,
                     m_text);
}

void KNScrollLabel::resizeEvent(QResizeEvent *event)
{
    //Update the widget size.
    QWidget::resizeEvent(event);
    //Update the animation parameter.
    updateAnimeParameters();
}

void KNScrollLabel::showEvent(QShowEvent *event)
{
    //Do the original show event.
    QWidget::showEvent(event);
    //Start the animation if it could.
    startAnime();
}

void KNScrollLabel::hideEvent(QHideEvent *event)
{
    //Do the original hide event.
    QWidget::hideEvent(event);
    //Stop the animation.
    stopAnime();
}

void KNScrollLabel::moveText()
{
    //Check moving direction first.
    if(m_movingLeft)
    {
        //If the current position is the most left,
        if(m_textX<=m_textLeftMostX)
        {
            //Change the direction.
            m_movingLeft=false;
            //Stop moving timer.
            m_move->stop();
            //Start short waiting timer.
            m_wait->setInterval(ShortWaiting);
            m_wait->start();
            return;
        }
        //Update the text position parameter.
        m_textX--;
        //Udpate the widget.
        update();
        return;
    }
    //Check if current position is the most right,
    if(m_textX>=contentsRect().x())
    {
        //Change the direction,
        m_movingLeft=true;
        //Stop moving timer.
        m_move->stop();
        //Start long waiting timer.
        m_wait->setInterval(LongWaiting);
        m_wait->start();
        return;
    }
    //Update the text position parameter.
    m_textX++;
    update();
}

void KNScrollLabel::stopTimers()
{
    //Stop all the timers.
    m_move->stop();
    m_wait->stop();
}

inline void KNScrollLabel::updateAnimeParameters()
{
    //Stop all the timer.
    stopTimers();
    //Reset the text left.
    m_textX=contentsRect().x();
    //Get the text width.
    int textWidth=fontMetrics().width(m_text)+m_glowRadius;
    //Check the text width is longer than the maximum display width.
    int maxDisplayWidth=rect().width()+m_glowRadius/2+1;
    if(textWidth>maxDisplayWidth)
    {
        //Set the tooltip.
        setToolTip(m_text);
        //Calculate most left X. This can control the right spacing.
        m_textLeftMostX=maxDisplayWidth-textWidth;
        //Check out the visible.
        if(isVisible())
        {
            //Reset the interval to long waiting.
            m_wait->setInterval(LongWaiting);
            //Start waiting timer.
            m_wait->start();
        }
        return;
    }
    //Clear the tooltip text.
    setToolTip("");
    //When the text width is shorter than width.
    m_textLeftMostX=0;
}
qreal KNScrollLabel::glowRadius() const
{
    return m_glowRadius;
}

void KNScrollLabel::setGlowRadius(const qreal &glowRadius)
{
    m_glowRadius = glowRadius;
}

void KNScrollLabel::startAnime()
{
    //Check out the parameter, check whether we need to scroll the content.
    if(m_textLeftMostX!=0)
    {
        //Start long waiting timer.
        m_wait->setInterval(LongWaiting);
        m_wait->start();
    }
}

void KNScrollLabel::stopAnime()
{
    //Stop all timers.
    stopTimers();
    //Reset the parameters.
    m_textX=contentsRect().x();
}

qreal KNScrollLabel::opacity() const
{
    return m_opacity;
}

void KNScrollLabel::setOpacity(const qreal &opacity)
{
    //Save the opacity.
    m_opacity = opacity;
    //Update the label.
    update();
}

QString KNScrollLabel::text() const
{
    return m_text;
}

void KNScrollLabel::setText(const QString &text)
{
    //Stop all the previous timer first.
    stopTimers();
    //Reset all the moving direction.
    m_movingLeft=true;
    //Save the text data.
    m_text=text;
    //Update animation parameters.
    updateAnimeParameters();
    //Update the text data.
    update();
}

QSize KNScrollLabel::sizeHint() const
{
    return QSize(width(), m_glowRadius+fontMetrics().height());
}
