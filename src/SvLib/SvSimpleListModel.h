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
   История
     17.03.2023 v1 Начал вести историю
*/
#ifndef SVSIMPLELISTMODEL_H
#define SVSIMPLELISTMODEL_H

#include <QAbstractListModel>

#define SV_SIMPLELISTMODEL_VERSION 1

class SvSimpleListModel : public QAbstractListModel
  {
    Q_OBJECT
  public:
    explicit SvSimpleListModel(QObject *parent = nullptr);

    //!
    //! \brief count Возвращает количество элементов в списке
    //! \return      Количество элементов в списке
    //!
    virtual int count() const = 0;

    //Определение функций, необходимых для объекта-списка
    virtual int      rowCount(const QModelIndex &parent) const override { Q_UNUSED(parent) return count(); }
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
  };


#endif // SVSIMPLELISTMODEL_H
