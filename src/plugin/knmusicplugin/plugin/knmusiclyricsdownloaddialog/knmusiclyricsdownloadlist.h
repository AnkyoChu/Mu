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

#ifndef KNMUSICLYRICSDOWNLOADLIST_H
#define KNMUSICLYRICSDOWNLOADLIST_H

#include <QIcon>
#include <QModelIndex>

#include "knconnectionhandler.h"

#include "knmusiclyricsdownloader.h"

#include <QWidget>

class QBoxLayout;
class QLabel;
class QListView;
class KNClockWheel;
class KNOpacityButton;
class KNProgressSlider;
class KNMusicLrcParser;
class KNMusicScrollLyrics;
class KNMusicLyricsBackend;
class KNMusicLyricsDetailListModel;
/*!
 * \brief The KNMusicLyricsDownloadList class is a content widget which could
 * getting the search keywords and display the searching progress and result.
 */
class KNMusicLyricsDownloadList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLyricsDownloadList widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLyricsDownloadList(QWidget *parent = 0);

    /*!
     * \brief Get the current lyrics text data.
     * \return The select lyrics text.
     */
    QString currentLyricsData();

signals:
    /*!
     * \brief This signal is emitted when there's any lyrics is valid for user.
     * And the okay button of the dialog should be shown.
     */
    void requireShowOkay();

    /*!
     * \brief This signal is emitted when there's none lyrics is valid. The okay
     * button should be hidden.
     */
    void requireHideOkay();

public slots:
    /*!
     * \brief Hide all the content widgets.
     */
    void hideAllWidgets();

    /*!
     * \brief Show the download widgets. Which means hide the lyrics lists and
     * show the waiting wheels.
     */
    void showDownloadWidgets();

    /*!
     * \brief Hide all the download widgets. Show the lyrics result list.
     */
    void hideDownloadWidgets();

    /*!
     * \brief Update the server searching progress.
     * \param current The current server number.
     * \param total The total server count.
     */
    void setDownloadServerText(int current, int total);

    /*!
     * \brief Set the lyrics search result list.
     * \param lyricsList The lyrics detail lists.
     */
    void setLyricsList(
        const QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionCurrentChanged(const QModelIndex &index);
    void onActionPlayStateChanged(const int &state);
    void onActionPlayNPauseClick();
    void onActionPositionChanged(const qint64 &position);

private:
    inline void linkBackend();
    inline void cutLinkBackend();
    KNConnectionHandler m_previewLinker;
    QIcon m_playIcon, m_pauseIcon;
    QWidget *m_container;
    QWidget *m_previewPlayer;
    KNClockWheel *m_clockWheel;
    QLabel *m_downloadServer;
    QString m_downloadServerText;
    QListView *m_lyricsList;
    KNMusicLrcParser *m_lrcParser;
    KNMusicLyricsDetailListModel *m_lyricsDetailListModel;
    KNMusicLyricsBackend *m_previewBackend;
    KNMusicScrollLyrics *m_scrollLyrics;
    KNOpacityButton *m_playNPause;
    KNProgressSlider *m_progress;
    bool m_progressPressed;
};

#endif // KNMUSICLYRICSDOWNLOADLIST_H
