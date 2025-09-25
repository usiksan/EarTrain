#include "CsTimeLineInfo.h"
#include "CsCell.h"
#include "CsParser.h"

#include <QTextBlock>



CsTimeLineInfo::CsTimeLineInfo() :
  mStartTick(0),
  mLineDuration(0)
  {

  }




void CsTimeLineInfo::blockParse(QTextCursor from, int *pStartTick)
  {
  QTextCursor tcr( CsParser::findNearestTime( from ) );
  if( !tcr.isNull() ) {
    parse( tcr.block().text() );
    if( pStartTick != nullptr )
      *pStartTick = lineStart();
    }
  }




void CsTimeLineInfo::parse(const QString &timeLine)
  {
  mTickList.clear();
  mLineDuration = 0;
  if( !timeLine.isEmpty() ) {
    QString timeValue( timeLine.mid(2).trimmed().split(' ').at(0).trimmed() );
    QStringList timeList( timeValue.split('/') );
    if( timeList.count() == 2 ) {
      bool ok;
      int startTick = timeList.at(0).toInt( &ok );
      if( ok && startTick != 0 )
        mStartTick = startTick;
      int stepCount = timeList.at(1).toInt();
      int stepBase;
      switch( stepCount ) {
        case 2 : stepBase = duraHalf; break;
        case 4 : stepBase = duraQuarter; break;
        case 8 : stepBase = duraEighth; break;
        case 16 : stepBase = duraSixteenth; break;
        case 32 : stepBase = duraThirtySecond; break;
        case 64 : stepBase = duraSixtyFourth; break;
        default : return;
        }

      //Fill note positions
      int tickCount = 0;
      for( int i = 2; i < timeLine.size(); i++ )
        if( timeLine.at(i) == QChar('|') || timeLine.at(i) == QChar('+') ) {
          CsTimeTick tt;
          tt.mPosition = i;
          tt.mIsTakt = timeLine.at(i) == QChar('|');
          if( tt.mIsTakt ) {
            tickCount = 0;
            tt.mIsQuartTakt = true;
            }
          else
            tt.mIsQuartTakt = (tickCount % (stepCount / 4 * stepBase)) == 0;
          if( i + 1 < timeLine.size() ) {
            if( timeLine.at(i+1) == QChar('_') ) {
              i++;
              tt.mDuration = stepBase / 2;
              }
            else if( timeLine.at(i+1) == QChar('.') ) {
              i++;
              tt.mDuration = stepBase / 4;
              }
            else tt.mDuration = stepBase;
            }
          else tt.mDuration = stepBase;

          mTickList.append( tt );

          tickCount += tt.mDuration;

          mLineDuration += tt.mDuration;
          }
      }
    }
  }




int CsTimeLineInfo::duration(int fromIndex, int toIndex) const
  {
  int dura = 0;
  while( fromIndex < mTickList.count() && fromIndex < toIndex ) {
    dura += mTickList.at(fromIndex).mDuration;
    fromIndex++;
    }
  return dura;
  }




// int CsTimeLineInfo::startFromLine(int index) const
//   {
//   int st = 0;
//   for( int i = 0; i < index && i < mTickList.count(); i++ )
//     st += mTickList.at(i).mDuration;
//   return st;
//   }
