#pragma once
#include <memory>
#include <QAbstractListModel>

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int boardSize() const;

    Q_INVOKABLE void newGame();
    Q_INVOKABLE bool save_step(bool dell, int en,int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void exxx();
    Q_INVOKABLE void save();
    Q_INVOKABLE bool load();
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool prv();
    Q_INVOKABLE bool nxt();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool kill(int fromX, int fromY, int toX, int toY, int index, int enemy);
    Q_INVOKABLE bool checkSpace(int fromX, int fromY, int toX, int toY);

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
//    struct Steps;
//    std::unique_ptr<Steps> step;
};
