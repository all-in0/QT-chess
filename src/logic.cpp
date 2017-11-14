#include <stdio.h>
#include "logic.h"
#include <iostream>
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>
#include <QString>
#include <fstream>
#include <QFileDialog>
#include <QFile>

struct Steps
{
  bool ifDel;
  int type;
  int fromX;
  int fromY;
  int toX;
  int toY;
  Steps *next;
};

int g_move;
int g_mv;
int g_moves;
Steps *g_steps;
Steps *g_load;
size_t g_len;

struct Figure
{
  int type;
  int x;
  int y;
};

struct Logic::Impl
{
  QList<Figure> figures;
 // QList<Step> steps;

  int findByPosition(int x, int y);
};

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) { 
      continue; 
    }
    return i;    
  }
  return -1;
}


Logic::Logic(QObject *parent)
  : QAbstractListModel(parent)
  , impl(new Impl()) 
{
   g_steps = NULL;
  g_move = 1;
  g_len = 0;
}

Logic::~Logic() {
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Type      , "type");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
  if (!modelIndex.isValid()) {
    return QVariant();
  }
    
  int index = static_cast<int>(modelIndex.row());
  
  if (index >= impl->figures.size() || index < 0) {
    return QVariant();
  }

  Figure & figure = impl->figures[index];
    
  switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
  }
  return QVariant(); 
}

void Logic::clear() {
  beginResetModel();
  impl->figures.clear();
  endResetModel();
}

void Logic::newGame()
{
    g_move = 1;
    if (g_steps!= NULL)
        delete g_steps;
    g_steps = NULL;
    g_len = 0;
    Logic::clear();
    beginInsertRows(QModelIndex(), 0, 31);
    impl->figures << Figure { 0, 0, 1 };
    impl->figures << Figure { 0, 1, 1 };
    impl->figures << Figure { 0, 2, 1 };
    impl->figures << Figure { 0, 3, 1 };
    impl->figures << Figure { 0, 4, 1 };
    impl->figures << Figure { 0, 5, 1 };
    impl->figures << Figure { 0, 6, 1 };
    impl->figures << Figure { 0, 7, 1 };
    impl->figures << Figure { 1, 0, 6 };
    impl->figures << Figure { 1, 1, 6 };
    impl->figures << Figure { 1, 2, 6 };
    impl->figures << Figure { 1, 3, 6 };
    impl->figures << Figure { 1, 4, 6 };
    impl->figures << Figure { 1, 5, 6 };
    impl->figures << Figure { 1, 6, 6 };
    impl->figures << Figure { 1, 7, 6 };
    impl->figures << Figure { 2, 7, 0 };
    impl->figures << Figure { 2, 0, 0 };
    impl->figures << Figure { 3, 7, 7 };
    impl->figures << Figure { 3, 0, 7 };
    impl->figures << Figure { 4, 1, 0 };
    impl->figures << Figure { 4, 6, 0 };
    impl->figures << Figure { 5, 1, 7 };
    impl->figures << Figure { 5, 6, 7 };
    impl->figures << Figure { 6, 2, 0 };
    impl->figures << Figure { 6, 5, 0 };
    impl->figures << Figure { 7, 2, 7 };
    impl->figures << Figure { 7, 5, 7 };
    impl->figures << Figure { 8, 3, 0 };
    impl->figures << Figure { 9, 3, 7 };
    impl->figures << Figure { 11, 4, 7 };
    impl->figures << Figure { 10, 4, 0 };
    endInsertRows();
}

bool Logic::load()
{
    QString fileName = NULL;
    Steps *tmp;
    char *magic = new char[5];
    int moves;

    g_load = NULL;
    if ((fileName = QFileDialog::getOpenFileName()) == NULL)
        return false;
    if (fileName != "") {
           QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
            return false;
    else {
        file.read(magic, sizeof(char) * 5);
       // printf("%s", magic);
        if (strcmp(magic, "*42*"))
            return false;
        file.read((char *)&moves, sizeof(int));
        for (int i = 0; i < (moves - 1); i++)
        {
        tmp = new Steps;
        file.read((char *)tmp, sizeof(Steps));
        tmp->next = g_load;
        g_load = tmp;
        }
        g_moves = moves;
     g_mv = 0;
     newGame();
//        for (int i = 0; i < 4; i++)
//        {
//            printf("%d | %d | %d\n", g_load->fromX , g_load->type, g_load->toX);
//            g_load = g_load->next;
//        }
    }
    }
    return true;
}

void Logic::save()
{
    QString fileName = NULL;
    Steps *tmp;

    if (g_move == 1)
        return ;
    if ((fileName = QFileDialog::getSaveFileName() + ".chess") == NULL)
        return ;
    tmp = g_steps;
    if (fileName != "") {
           QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
            return ;
    else {
        file.write("*42*", 5);
        file.write((char *)&g_move, sizeof(int));
        while(tmp != NULL)
        {
        file.write((char *)tmp, sizeof(Steps));
        tmp = tmp->next;
        }
    }
    }
}

void Logic::exxx()
{
    exit(1);
}

//void Logic::new() {

//}

bool Logic::checkSpace(int fromX, int fromY, int toX, int toY)
{
  while (1)
  {
    if (fromX > toX)
        fromX--;
    else if (fromX < toX)
        fromX++;
    if (fromY > toY)
        fromY--;
    else if (fromY < toY)
        fromY++;
    if (fromX == toX && fromY == toY) return true;
    if ((impl->findByPosition(fromX, fromY)) >= 0) return false;
  }
  return true;
}

bool Logic::kill(int fromX, int fromY, int toX, int toY, int index, int enemy)
{
    if (impl->figures[index].type == 0 && (((toY - fromY) == 1) && ((toX - fromX) == 1 || (toX - fromX) == -1)))//first move +1
    {
        beginRemoveRows(QModelIndex(), enemy, enemy);
        impl->figures.removeAt(enemy);
        endRemoveRows();
        return true;
    }
    else if (impl->figures[index].type == 1 && (((toY - fromY) == -1) && ((toX - fromX) == 1 || (toX - fromX) == -1)))
    {
        beginRemoveRows(QModelIndex(), enemy, enemy);
        impl->figures.removeAt(enemy);
        endRemoveRows();
        return true;
    }
    if ((impl->figures[index].type == 2 || impl->figures[index].type == 3) && (toX == fromX || toY == fromY))
    {
        beginRemoveRows(QModelIndex(), enemy, enemy);
        impl->figures.removeAt(enemy);
        endRemoveRows();
        return true;
    }
    if (impl->figures[index].type == 4 || impl->figures[index].type == 5)
    {
            if ((((toX - fromX) == 2 || (toX - fromX) == -2) && ((toY - fromY) == 1 || (toY - fromY) == -1)) || (((toY - fromY) == 2 || (toY - fromY) == -2) && ((toX - fromX) == 1 || (toX - fromX) == -1)))
            {
                beginRemoveRows(QModelIndex(), enemy, enemy);
                impl->figures.removeAt(enemy);
                endRemoveRows();
                return true;
            }
    }
    if ((impl->figures[index].type == 6 || impl->figures[index].type == 7))
    {
            if (((toX - fromX) == (toY - fromY)) || ((toX - fromX) == ((toY - fromY) * -1)))
            {
                beginRemoveRows(QModelIndex(), enemy, enemy);
                impl->figures.removeAt(enemy);
                endRemoveRows();
                return true;
            }
  }
    if (impl->figures[index].type == 8 || impl->figures[index].type == 9)
    {
        if ((((toX - fromX) == (toY - fromY)) || ((toX - fromX) == ((toY - fromY) * -1))) || (toX == fromX || toY == fromY))
        {
            beginRemoveRows(QModelIndex(), enemy, enemy);
            impl->figures.removeAt(enemy);
            endRemoveRows();
            return true;
        }
    }
    if (impl->figures[index].type == 10 || impl->figures[index].type == 11)
    {
        if (((toX-fromX) > 1 || (toX - fromX) < -1) || ((toY-fromY) > 1 || (toY - fromY) < -1)) return false;
        beginRemoveRows(QModelIndex(), enemy, enemy);
        impl->figures.removeAt(enemy);
        endRemoveRows();
        return true;
    }
    return false;
}

uint8_t	*memjoin(uint8_t *s1, uint8_t *s2, size_t size)
{
    size_t		i;
    size_t		b;
    uint8_t	*str;

    str = new uint8_t [g_len + size];
    i = 0;
    while (i < g_len)
    {
        str[i] = s1[i];
        i++;
    }
    b = 0;
    while (b <= size)
    {
        str[i] = s2[b];
        i++;
        b++;
    }
    if (s1 != NULL)
     delete s1;
    if (s2 != NULL)
        free (s2);
    g_len += size;
    return (str);
}

bool Logic::save_step(bool dell, int en, int fromX, int fromY, int toX, int toY)
{
  Steps *nStep = new Steps;

  nStep->ifDel = dell;
  nStep->type = en;
  nStep->fromX = fromX;
  nStep->fromY = fromY;
  nStep->toX = toX;
  nStep->toY = toY;
  nStep->next = g_steps;
  g_steps = nStep;
//  uint8_t *rr = (uint8_t *)nStep;
  return true;
}

bool Logic::move(int fromX, int fromY, int toX, int toY) {
  int index = impl->findByPosition(fromX, fromY);
  int enemy = impl->findByPosition(toX, toY);
  bool fl = false;

  if (index < 0 || (fromX == toX && fromY == toY)) return false;
  if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE) {
    return false;
  }
  if ((g_move % 2) == (impl->figures[index].type % 2)) return false;

  if (impl->figures[index].type != 4 && impl->figures[index].type != 5)
  {
       if(((checkSpace(fromX, fromY, toX, toY)) == false))
            return false;
  }

  if (enemy >= 0)
  {
      if ((impl->figures[index].type % 2) == (impl->figures[enemy].type % 2)) return false;
      if ((kill(fromX, fromY, toX, toY, index, enemy)) == false) return false;
      if (enemy < index)
          index--;
        fl = true;
  }
 else{
  if (impl->figures[index].type == 0 && (((toY - fromY) != 1) || toX != fromX))
  {
    if ((toY - fromY) == 2 && toX == fromX && fromY == 1) ; //check 6 && 2
    else 
      return false;
  }
  if (impl->figures[index].type == 1 && (((toY - fromY) != -1) || toX != fromX))
  {
    if ((toY - fromY) == -2 && toX == fromX && fromY == 6) ; //check 6 && 2
    else 
      return false;
  }
  if ((impl->figures[index].type == 2 || impl->figures[index].type == 3) && (toX != fromX && toY != fromY))
      return false;
  if (impl->figures[index].type == 4 || impl->figures[index].type == 5)
  {
          if (((toX - fromX) == 2 || (toX - fromX) == -2) && ((toY - fromY) == 1 || (toY - fromY) == -1));
          else if (((toY - fromY) == 2 || (toY - fromY) == -2) && ((toX - fromX) == 1 || (toX - fromX) == -1));
          else return false;
  }
  if ((impl->figures[index].type == 6 || impl->figures[index].type == 7))
  {
          if (((toX - fromX) == (toY - fromY)) || ((toX - fromX) == ((toY - fromY) * -1)));
          else
              return false;
}
  if (impl->figures[index].type == 8 || impl->figures[index].type == 9)
  {
      if (((toX - fromX) == (toY - fromY)) || ((toX - fromX) == ((toY - fromY) * -1)));
      else if (toX == fromX || toY == fromY);
      else return false;
  }
  if (impl->figures[index].type == 10 || impl->figures[index].type == 11)
  {
      if (((toX-fromX) > 1 || (toX - fromX) < -1) || ((toY-fromY) > 1 || (toY - fromY) < -1)) return false;
  }
  }
  if ((impl->figures[index].type == 0 && toY == 7) || (impl->figures[index].type == 1 && toY == 0))
    impl->figures[index].type == 1 ? impl->figures[index].type = 9 : impl->figures[index].type = 8;
  impl->figures[index].x = toX;
  impl->figures[index].y = toY;
  save_step(fl, impl->findByPosition(toX, toY), fromX, fromY, toX, toY);
  QModelIndex topLeft = createIndex(index, 0);
  QModelIndex bottomRight = createIndex(index, 0);
  emit dataChanged(topLeft, bottomRight);
  g_move++;
  return true;
}

bool Logic::nxt()
{
    Steps *tmp = g_load;
    if (g_moves > g_mv)
        g_mv++;
    newGame();
    for (int i = 0; i < g_mv && tmp != NULL; i++)
    {
        move(tmp->fromX, tmp->fromY, tmp->toX, tmp->toY);
        tmp = tmp->next;
    }
    return true;
}

bool Logic::prv()
{
    Steps *tmp = g_load;
    if (g_mv > 0)
        g_mv--;
    else
        return false;
    newGame();
    for (int i = 0; i < (g_mv - 1) && tmp != NULL; i++)
    {
        move(tmp->fromX, tmp->fromY, tmp->toX, tmp->toY);
        tmp = tmp->next;
    }
    return true;
}
