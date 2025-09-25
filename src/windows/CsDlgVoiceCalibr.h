#ifndef CSDLGVOICECALIBR_H
#define CSDLGVOICECALIBR_H

#include <QDialog>
#include <QLineEdit>

class CsDlgVoiceCalibr : public QDialog
  {
    Q_OBJECT

    QLineEdit *mIdealPitch;
    QLineEdit *mCurrentPitch;
  public:
    CsDlgVoiceCalibr( QWidget *parent = nullptr );
  };

#endif // CSDLGVOICECALIBR_H
