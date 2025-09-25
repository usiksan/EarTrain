/*
   Проект "Библиотека общих элементов"
   Автор
     Сибилев А.С.

     www.salilab.ru
     www.salilab.com
   Описание
     SvSimpleListModel класс для создания списков объектов, порожденных от QObject
     для удобства экспорта в qml

     Например, если Track является потомком QObject, то список этих объектов может быть
     образован следующим образом:
     class TrackList : public SvSimpleListModel
       {
         Q_OBJECT

         QList<Track*> mTrackList;
       public:
         virtual int count() const { return mTrackList.count(); }

       public slots:
         Track *track( int row ) { return mTrackList[row]; }
       };
*/
#include "SvSimpleListModel.h"



SvSimpleListModel::SvSimpleListModel(QObject *parent)
  : QAbstractListModel{parent}
  {

  }



QVariant SvSimpleListModel::data(const QModelIndex &index, int role) const
  {
  Q_UNUSED(index)
  Q_UNUSED(role)
  return QVariant();
  }



QHash<int, QByteArray> SvSimpleListModel::roleNames() const
  {
  QHash<int, QByteArray> roles;
  roles.insert( Qt::UserRole + 1, "svSimple" );
  return roles;
  }
