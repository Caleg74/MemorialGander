#ifndef CREATEPDF_H
#define CREATEPDF_H

#include <QTextDocument>
#include <QPrinter>
#include <QTextStream>
#include <QPainter>
#include "gymnastranking.h"

class CreatePdf : public QObject
{
    Q_OBJECT
public:
    CreatePdf();

    void retriveGymnastList();

public slots:
    void PrintRanking();
    void PrintLastApparatusStartOrder();

private:

    /** Creates a pdf for the Men's allround ranking.
     * @param p_bHFImages true=with Header/Footer sponsor images
     */
    void PrintMenAllround(bool p_bHFImages);

    /** Creates a pdf for the Women's allround ranking.
     * @param p_bHFImages true=with Header/Footer sponsor images
     */
    void PrintWomenAllround(bool p_bHFImages);

//    /** Creates a pdf for the Men's CityTrophy ranking.
//     * @param p_bHFImages true=with Header/Footer sponsor images
//     * @obsolete
//     */
//    void PrintMenCityTrophy(bool p_bHFImages);

//    /** Creates a pdf for the Women's CityTrophy ranking.
//     * @param p_bHFImages true=with Header/Footer sponsor images
//     * @obsolete
//     */
//    void PrintWomenCityTrophy(bool p_bHFImages);

    void PrintAllSingleApparatusMen(bool p_bHFImages);
    void PrintAllSingleApparatusWomen(bool p_bHFImages);

    void PrintHeader(bool p_bHFImages, QTextStream& out);
    void PrintFooter(bool p_bHFImages, QTextStream& out);
    void PrintMenTableTitle(QTextStream& out);
    void PrintMenTableBody(QTextStream& out);
    void PrintWomenTableTitle(QTextStream& out);
    void PrintWomenTableBody(QTextStream& out);
    void PrintSingleMApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusMen p_eApparatus);
    void PrintSingleMApparatusTableBody(QTextStream& out);
    void PrintSingleWApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusWomen p_eApparatus);
    void PrintSingleWApparatusTableBody(QTextStream& out);

    void AddHeader(bool p_bHFImages);
    void AddFooter(bool p_bHFImages);

    void AddPageNumber();
    void DrawHtml(const QString& html);

    QTextStream m_textStream;

    QPainter m_painter;

    QPrinter m_printer;

    GymnastRanking m_cRank;

    int m_iPageNumber;
};

#endif // CREATEPDF_H
