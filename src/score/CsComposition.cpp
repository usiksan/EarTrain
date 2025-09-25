#include "CsComposition.h"
#include "CsEarTrainer.h"
#include "CsSongParser.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif


#define CS_CELL_TYPE       (Qt::UserRole + 1) //!< Режим юнита (имя в текущей конфигурации)
#define CS_CELL_TEXT       (Qt::UserRole + 2) //!< Текстовый идентификатор юнита в программе
#define CS_CELL_TEXT_SIZE  (Qt::UserRole + 3) //!< Тип юнита (мотор, датчик)
#define CS_CELL_POS_X      (Qt::UserRole + 4) //!< Номер канала, к которому подключен в контроллере юнит
#define CS_CELL_POS_Y      (Qt::UserRole + 5) //!< Cell pos Y
#define CS_CELL_WIDTH      (Qt::UserRole + 6) //!< Cell width
#define CS_CELL_HEIGHT     (Qt::UserRole + 7)  //!< Cell height
#define CS_CELL_ALIGN      (Qt::UserRole + 8)  //!< Text align
#define CS_CELL_HIGH       (Qt::UserRole + 9)  //!< Highlight cell
#define CS_CELL_WRONG      (Qt::UserRole + 10) //!< Wrong cell indicator
#define CS_CELL_COLOR_BACK (Qt::UserRole + 11) //!< Rectangle background color
#define CS_CELL_COLOR_TEXT (Qt::UserRole + 12) //!< Text color
#define CS_CELL_NOTE_LINE  (Qt::UserRole + 13) //!< Note line index
#define CS_CELL_NOTE_SHARP (Qt::UserRole + 14) //!< Note sharp flag
#define CS_CELL_ACTIVE     (Qt::UserRole + 15) //!< Active cell flag



CsComposition::CsComposition(QObject *parent) :
  SvSimpleListModel{parent},
  mFragmentBegin(0),
  mFragmentEnd(0),
  mPlayPos(CsViewSettings::CsCtMarker, 0, 0, 0, 0, 0, 1 )
  {
  clear();
  }




QString CsComposition::fileName() const
  {
  if( mFilePath.isEmpty() )
    return mFilePath;
  QFileInfo info(mFilePath);
  return info.fileName();
  }




QString CsComposition::fileBasePath() const
  {
  if( mFilePath.isEmpty() )
    return mFilePath;
  QFileInfo info(mFilePath);
  return info.absolutePath();
  }




int CsComposition::width() const
  {
  return CsEarTrainer::instance()->visualWidth();
  }




int CsComposition::height() const
  {
  return CsEarTrainer::instance()->visualHeight();
  }





void CsComposition::fragmentBeginSet(int newFragmentBegin )
  {
  if( mFragmentBegin == newFragmentBegin ) {
    //Remove selection
    mFragmentBegin = -1;
    mFragmentEnd = count();
    }
  else {
    //When we press fragment begin we move fragment at new place (or make 1 line fragment if no previous fragment selected)
    int size = mFragmentBegin < 0 ? 1 : CsEarTrainer::instance()->fragmentSize( mFragmentBegin, mFragmentEnd );
    mFragmentBegin = newFragmentBegin;
    mFragmentEnd = CsEarTrainer::instance()->fragmentPosition( mFragmentBegin, size, true );
    }
  emit fragmentChanged();
  }




void CsComposition::fragmentEndSet(int newFragmentEnd)
  {
  if( mFragmentEnd == newFragmentEnd ) {
    //Remove selection
    mFragmentBegin = -1;
    mFragmentEnd = count();
    }
  else {
    mFragmentEnd = newFragmentEnd;
    if( !(mFragmentBegin >= 0 && newFragmentEnd > mFragmentBegin) ) {
      //Select fragment as single line with end at newFragmentEnd
      mFragmentBegin = CsEarTrainer::instance()->fragmentPosition( newFragmentEnd, 1, false );
      if( mFragmentBegin < 0 )
        mFragmentEnd = count();
      }
    }
  emit fragmentChanged();
  }







void CsComposition::playLoopSet(bool newPlayLoop)
  {
  if( mPlayLoop == newPlayLoop )
    return;
  mPlayLoop = newPlayLoop;
  // if( newPlayEnd > 0 )
  //   mPlayEnd = mLineList.at(mLineIndex)->cellCursorStop();
  emit playLoopChanged();
  }




double CsComposition::playBarY() const
  {
  double h(height());
  double y(playPosY());
  return y / h;
  }






bool CsComposition::isMobile() const
  {
#ifdef Q_OS_ANDROID
  return true;
#else
  return false;
#endif
  }



void CsComposition::clear()
  {
  beginResetModel();
  endResetModel();

  mFragmentBegin = mFragmentEnd = 0;
  emit fragmentChanged();

  mPKey[0] = mPKey[1] = mPKey[2] = mPKey[3] = 0;
  emit pkey0Changed();
  emit pkey1Changed();
  emit pkey2Changed();
  emit pkey3Changed();

  mPlayPos = CsCell(CsViewSettings::CsCtMarker, 0, 0, 0, 0, 0, 1 );
  emit playPosYChanged();
  emit playPosChanged();
  }




int CsComposition::count() const
  {
  return CsEarTrainer::instance()->cellList().count();
  }




int CsComposition::rowCount(const QModelIndex &parent) const
  {
  Q_UNUSED(parent)
  return count();
  }




QVariant CsComposition::data(const QModelIndex &index, int role) const
  {
  const CsCellList &cellList( CsEarTrainer::instance()->cellList() );
  if( !index.isValid() || index.row() >= cellList.count() || index.row() < 0 || role <= Qt::UserRole )
    return QVariant{};

  auto cell = cellList.at( index.row() );
  switch( role ) {
    case CS_CELL_TYPE            : return cell.cellType();
    case CS_CELL_TEXT            : return cell.text();
    case CS_CELL_TEXT_SIZE       : return cell.textSize();
    case CS_CELL_POS_X           : return cell.visualPosX();
    case CS_CELL_POS_Y           : return cell.visualPosY();
    case CS_CELL_WIDTH           : return cell.visualWidth();
    case CS_CELL_HEIGHT          : return cell.visualHeight();
    case CS_CELL_ALIGN           : return cell.textAlign();
    case CS_CELL_NOTE_LINE       : return cell.noteLineIndex();
    case CS_CELL_NOTE_SHARP      : return cell.noteSharp();
    case CS_CELL_ACTIVE          : return mActiveCells.contains( index.row() );
    }
  return QVariant{};
  }





QHash<int, QByteArray> CsComposition::roleNames() const
  {
  QHash<int, QByteArray> roles;
  roles.insert( CS_CELL_TYPE,          "cellType" );
  roles.insert( CS_CELL_TEXT,          "cellText" );
  roles.insert( CS_CELL_TEXT_SIZE,     "cellTextSize" );
  roles.insert( CS_CELL_POS_X,         "cellPosX" );
  roles.insert( CS_CELL_POS_Y,         "cellPosY" );
  roles.insert( CS_CELL_WIDTH,         "cellWidth" );
  roles.insert( CS_CELL_HEIGHT,        "cellHeight" );
  roles.insert( CS_CELL_ALIGN,         "cellAlign" );
  roles.insert( CS_CELL_NOTE_LINE,     "cellNoteLine" );
  roles.insert( CS_CELL_NOTE_SHARP,    "cellNoteSharp" );
  roles.insert( CS_CELL_ACTIVE,        "cellActive" );
  return roles;
  }








void CsComposition::songSet(const QString &song)
  {
  mSong = song;
  songBuild();
  }





//!
//! \brief playStart  Start play of song
//! \param trainLevel 0 - simple play, 1 - play without sound on track 0, 2 - play without sound on track 0 and wait right user key pressed
//!
void CsComposition::playStart(int trainLevel)
  {
  CsEarTrainer::instance()->playFragment( mFragmentBegin, mFragmentEnd, trainLevel, mPlayLoop );
  }





//!
//! \brief playStop Play stop
//!
void CsComposition::playStop()
  {
  CsEarTrainer::instance()->stop();
  }







//!
//! \brief midiNote User play note with screen keyboard.
//! \param pitch    Note pitch
//! \param velo     Note velo
//!
void CsComposition::kbdNote(int pitch, int velo)
  {
  //Synth note
  emit midiNote( 0, pitch, velo );
  }






//!
//! \brief pianoKeySet Show code color on piano key keyboard
//! \param pitch       Key pitch
//! \param code        Code: one of 0 - not active, 1 - green, 2 - blue, 3 - red
//!
void CsComposition::pianoKeySet(int pitch, int code)
  {
  if( pitch >= 12 * 3 && pitch < 12 * 8 ) {
    //Visual keyboard contains only 5 octaves, so clip into 5-octave interval
    pitch -= 12 * 3;
    //Each octave shifts code on 2 bits
    int pkeyShift = pitch / 12 * 2;
    //Note of pitch
    int note = pitch % 12;
    //In which mPKey resides this note
    int pkeyIndex = note / 3;
    //Correct shift
    pkeyShift += (note % 3) * 10;
    //Build clear mask for pitch position...
    int clearMask = ~(3 << pkeyShift);
    //... and build code
    code <<= pkeyShift;
    //Update mPKey
    mPKey[pkeyIndex] = (mPKey[pkeyIndex] & clearMask) | code;
    //Signal that mPKey changed
    switch( pkeyIndex ) {
      case 0 : emit pkey0Changed(); break;
      case 1 : emit pkey1Changed(); break;
      case 2 : emit pkey2Changed(); break;
      case 3 : emit pkey3Changed(); break;
      }
    }
  }




//!
//! \brief cellTick  Active and/or passive cell changed
//! \param tickIndex Tick count from begin song when cell changed
//!
void CsComposition::cellTick(int tickIndex)
  {
  if( tickIndex < 0 ) {
    //Deactivate all cells
    QSet<int> activeCells(mActiveCells);
    mActiveCells.clear();
    for( auto cell : std::as_const(activeCells) ) {
      emit dataChanged( index(cell), index(cell), QVector<int>( {CS_CELL_ACTIVE} ) );
      }
    mPlayPos = CsCell(CsViewSettings::CsCtMarker, 0, 0, 0, 0, 0, 1 );
    emit playPosYChanged();
    emit playPosChanged();
    }
  else {
    if( CsEarTrainer::instance()->cellContains(tickIndex) ) {
      //List of deactivated cells
      QList<int> cellList( CsEarTrainer::instance()->cellDeactivate().values(tickIndex) );
      if( cellList.count() ) {
        //There are cells to deactivate
        for( auto cell : cellList ) {
          //Find cell index in currently active cell list
          if( mActiveCells.contains(cell) ) {
            mActiveCells.remove(cell);
            emit dataChanged( index(cell), index(cell), QVector<int>( {CS_CELL_ACTIVE} ) );
            }
          }
        }

      //List of activated cells
      cellList = CsEarTrainer::instance()->cellActivate().values(tickIndex);
      if( cellList.count() ) {
        //There are cells to activate
        for( auto cell : cellList ) {
          //Append new cell in active cell list only if it is always not in list
          if( !mActiveCells.contains(cell) ) {
            mActiveCells.insert(cell);
            emit dataChanged( index(cell), index(cell), QVector<int>( {CS_CELL_ACTIVE} ) );

            const CsCell csCell( CsEarTrainer::instance()->cellList().at(cell) );
            if( csCell.cellType() == CsViewSettings::CsCtMarker ) {
              mPlayPos = csCell;
              mPlayPosX = mPlayPos.visualPosX();
              emit playPosChanged();
              emit playPosYChanged();
              }
            }
          }
        }
      }
    else {
      int posx = mPlayPos.visualTickPosX( tickIndex );
      if( posx != mPlayPosX ) {
        mPlayPosX = posx;
        emit playPosChanged();
        }
      }
    }
  }




void CsComposition::songBuild()
  {
  //Rebuild song model
  beginResetModel();
  QTextDocument doc(mSong);
  CsEarTrainer::instance()->parse( fileBasePath(), &doc );
  emit whChanged();
  endResetModel();

  //Remove selection
  mFragmentBegin = -1;
  mFragmentEnd = count();
  emit fragmentChanged();

  //Off all pressed keys
  mPKey[0] = mPKey[1] = mPKey[2] = mPKey[3] = 0;
  emit pkey0Changed();
  emit pkey1Changed();
  emit pkey2Changed();
  emit pkey3Changed();

  //Remove play position
  mPlayPos = CsCell(CsViewSettings::CsCtMarker, 0, 0, 0, 0, 0, 1 );
  emit playPosYChanged();
  emit playPosChanged();
  }

