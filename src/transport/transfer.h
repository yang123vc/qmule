
#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include <QSharedPointer>
#include <QtCore/QStringList>

#include "qtlibtorrent/qtorrenthandle.h"
#include "qtlibed2k/qed2khandle.h"

/**
 * Data transfer handle
 */
class Transfer
{
public:
    enum Type {
        BITTORRENT,
        ED2K,
        UNDEFINED
    };

    Transfer();
    Transfer(const QTorrentHandle& h);
    Transfer(const QED2KHandle& h);
    bool operator==(const Transfer& t) const;
    bool operator<(const Transfer& t) const;
    QTorrentHandle torrentHandle() const;
    QED2KHandle ed2kHandle() const;
    Type type() const;

    QString hash() const;
    QString name() const;
    QString save_path() const;
    QString firstFileSavePath() const;
    QString creation_date() const;
    QString comment() const;
    QString next_announce() const;
    TransferState state() const;
    TransferStatus status() const;
    TransferInfo get_info() const;
    qreal download_payload_rate() const;
    qreal upload_payload_rate() const;
    int queue_position() const;
    float progress() const;
    float distributed_copies() const;
    int num_files() const;
    int num_seeds() const;
    int num_peers() const;
    int num_complete() const;
    int num_incomplete() const;
    int num_connections() const;
    int upload_limit() const;
    int download_limit() const;
    int connections_limit() const;
    QString current_tracker() const;
    TransferSize actual_size() const;
    TransferSize total_done() const;
    TransferSize total_wanted_done() const;
    TransferSize total_wanted() const;
    TransferSize total_failed_bytes() const;
    TransferSize total_redundant_bytes() const;
    TransferSize total_payload_upload() const;
    TransferSize total_payload_download() const;
    TransferSize all_time_upload() const;
    TransferSize all_time_download() const;
    qlonglong active_time() const;
    qlonglong seeding_time() const;
    bool is_valid() const;
    bool is_seed() const;
    bool is_paused() const;
    bool is_queued() const;
    bool is_checking() const;
    bool has_metadata() const;
    bool priv() const;
    bool super_seeding() const;
    bool is_sequential_download() const;
    TransferBitfield pieces() const;
    void downloading_pieces(TransferBitfield& bf) const;
    void piece_availability(std::vector<int>& avail) const;
    std::vector<int> piece_priorities() const;
    TransferSize total_size() const;
    TransferSize piece_length() const;
    bool extremity_pieces_first() const;
    void file_progress(std::vector<TransferSize>& fp) const;
    QList<QDir> incompleteFiles() const;
    std::vector<int> file_priorities() const;
    QString filepath_at(unsigned int index) const;
    QString filename_at(unsigned int index) const;
    TransferSize filesize_at(unsigned int index) const;
    std::vector<int> file_extremity_pieces_at(unsigned int index) const;
    QStringList url_seeds() const;
    QStringList absolute_files_path() const;
    void get_peer_info(std::vector<PeerInfo>& peers) const;
    std::vector<AnnounceEntry> trackers() const;

    void pause() const;
    void resume() const;
    void move_storage(const QString& path) const;
    void rename_file(unsigned int index, const QString& new_name) const;
    void prioritize_files(const std::vector<int> priorities) const;
    void prioritize_extremity_pieces(bool p) const;
    void prioritize_extremity_pieces(bool p, unsigned int index) const;
    void set_tracker_login(const QString& login, const QString& passwd) const;
    void flush_cache() const;
    void force_recheck() const;
    void force_reannounce() const;
    void add_url_seed(const QString& url) const;
    void remove_url_seed(const QString& url) const;
    void connect_peer(const PeerEndpoint& ep) const;
    void set_peer_upload_limit(const PeerEndpoint& ep, long limit) const;
    void set_peer_download_limit(const PeerEndpoint& ep, long limit) const;
    void add_tracker(const AnnounceEntry& url) const;
    void replace_trackers(const std::vector<AnnounceEntry>& trackers) const;
    void queue_position_up() const;
    void queue_position_down() const;
    void queue_position_top() const;
    void queue_position_bottom() const;
    void super_seeding(bool ss) const;
    void set_sequential_download(bool sd) const;
    void set_eager_mode(bool b) const;

private:
    QSharedPointer<TransferBase> m_delegate;
};

#endif
