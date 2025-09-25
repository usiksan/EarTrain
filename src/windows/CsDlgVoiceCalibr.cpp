#include "CsDlgVoiceCalibr.h"
#include "audioFile/CsMelodyDetector.h"

#include <QFormLayout>
#include <QPushButton>

CsDlgVoiceCalibr::CsDlgVoiceCalibr(QWidget *parent)
  : QDialog(parent)
  {
  QFormLayout *layout = new QFormLayout();
  mIdealPitch = new QLineEdit();
  mIdealPitch->setReadOnly(true);

  mCurrentPitch = new QLineEdit();
  mCurrentPitch->setReadOnly(true);

  QPushButton *calibrButton = new QPushButton( tr("Калибр") );

  layout->addRow( tr("Нота"), mIdealPitch );
  layout->addRow( tr("Текущая"), mCurrentPitch );
  layout->addRow( calibrButton );

  setLayout( layout );

  connect( CsMelodyDetector::instance(), &CsMelodyDetector::idealPitchChanged, mIdealPitch, &QLineEdit::setText );
  connect( CsMelodyDetector::instance(), &CsMelodyDetector::realPitchChanged, mCurrentPitch, &QLineEdit::setText );
  connect( calibrButton, &QPushButton::pressed, CsMelodyDetector::instance(), &CsMelodyDetector::calibr );
  connect( calibrButton, &QPushButton::released, CsMelodyDetector::instance(), &CsMelodyDetector::calibrStop );
  }
