#ifndef SAVESCORE_H
#define SAVESCORE_H

#include <QObject>

class SaveScore : public QObject
{
    Q_OBJECT
public:
    explicit SaveScore(QObject *parent = nullptr);

public slots:
    bool onSaveScore(QString p_strGymnastFullTxt,
                     QString p_strApparatus,
                     QString p_strDifficultyScore,
                     QString p_strExecutionScore,
                     QString p_strPenaltyScore,
                     QString p_strFinalScore,
                     bool p_bFinalApparatus);

signals:
    void messageRequested(const QString &title, const QString &text);

private:

    float CheckDifficultyScore(QString p_strInputVal, bool* bOk);
    float CheckExecutionScore(QString p_strInputVal, bool* bOk);
    float CheckPenaltyScore(QString p_strInputVal, bool* bOk);

    float CheckFinalScore(QString p_strInputVal, bool* bOk);

    bool SendToDb(QString& p_strGymnastFullTxt,
                  QString& p_strApparatus,
                  float p_fDifficultyScore,
                  float p_fExecutionScore,
                  float p_fPenaltyScore,
                  float p_fFinalScore,
                  bool p_bFinalApparatus);

};

#endif // SAVESCORE_H
