#include "createpdf.h"
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>


CreatePdf::CreatePdf()
 : QObject()
// , m_doc()
 , m_printer(QPrinter::PrinterResolution)
 , m_cRank()
 , m_iPageNumber(0)
{
}

void CreatePdf::retriveGymnastList()
{
    m_cRank.createLists();
}

void CreatePdf::PrintRanking()
{
    retriveGymnastList();
    // 1st update the scores
    m_cRank.updateAllroundScores();

    // print 2 files,with and without header/footer
    for (int i=0; i<2; i++)
    {
        bool bWithLogo = (i==0); // 1st time with logo

        QString strFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

        if (bWithLogo)
            strFileName += "/MemorialGander2024.pdf";
        else
            strFileName += "/MemorialGander2024_NoImage.pdf";

        m_printer.setOutputFormat(QPrinter::PdfFormat);
        m_printer.setPageSize(QPageSize::A4);
        m_printer.setOutputFileName(strFileName);

        if (! m_painter.begin(&m_printer)) { // failed to open file
            qWarning("failed to open file, is it writable?");
            return;
        }

       m_iPageNumber = 1;
       PrintMenAllround(bWithLogo);

        if (! m_printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
        }

        PrintWomenAllround(bWithLogo);

        if (! m_printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
        }

        PrintAllSingleApparatusMen(bWithLogo);

        PrintAllSingleApparatusWomen(bWithLogo);

        m_painter.end();

        qDebug() << "File created in " << strFileName;
    }
}

void CreatePdf::PrintLastApparatusStartOrder()
{
    retriveGymnastList();
    // 1st update the scores
    //m_cRank.updateAllroundScores();

    QString strFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    strFileName += "/MemorialGander2024.pdf";

    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setOutputFileName(strFileName);

    if (! m_painter.begin(&m_printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    m_iPageNumber = 1;
    //PrintMenAllround(true);

    m_painter.end();

    qDebug() << "File created in " << strFileName;
}
void CreatePdf::PrintMenAllround(bool p_bHFImages)
{
    QString html;
    QTextStream out(&html);

    PrintHeader(p_bHFImages, out);
    PrintMenTableTitle(out);
    PrintMenTableBody(out);
    PrintFooter(p_bHFImages, out);

    out.flush();
    DrawHtml(html);
}

void CreatePdf::PrintWomenAllround(bool p_bHFImages)
{
    QString html;
    QTextStream out(&html);

    PrintHeader(p_bHFImages, out);
    PrintWomenTableTitle(out);
    PrintWomenTableBody(out);
    PrintFooter(p_bHFImages, out);

    out.flush();
    DrawHtml(html);
}

void CreatePdf::PrintAllSingleApparatusMen(bool p_bHFImages)
{
    for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AMApparatusMax; apparatus++)
    {
        QString html;
        QTextStream out(&html);

        PrintHeader(p_bHFImages, out);
        m_cRank.updateSingleScores(static_cast<ApparatusList::EApparatusMen>(apparatus));
        PrintSingleMApparatusTableTitle(out, static_cast<ApparatusList::EApparatusMen>(apparatus));
        PrintSingleMApparatusTableBody(out);
        PrintFooter(p_bHFImages, out);

        out.flush();
        DrawHtml(html);

        if (! m_printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
        }
    }
}

void CreatePdf::PrintAllSingleApparatusWomen(bool p_bHFImages)
{
    for (int apparatus = ApparatusList::AGFirstApparatus; apparatus < ApparatusList::AWApparatusMax; apparatus++)
    {
        QString html;
        QTextStream out(&html);

        PrintHeader(p_bHFImages, out);
        m_cRank.updateSingleScores(static_cast<ApparatusList::EApparatusWomen>(apparatus));
        PrintSingleWApparatusTableTitle(out, static_cast<ApparatusList::EApparatusWomen>(apparatus));
        PrintSingleWApparatusTableBody(out);
        PrintFooter(p_bHFImages, out);

        out.flush();
        DrawHtml(html);

        if (apparatus != (ApparatusList::AWApparatusMax-1))
        {
            if (! m_printer.newPage()) {
                qWarning("failed in flushing page to disk, disk full?");
            }
        }
    }
}

void CreatePdf::PrintHeader(bool p_bHFImages, QTextStream& out)
{
    // Add header
    QFile fileIn(":/pdf/IN_NoHeader.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strHeader(&fileIn);
    out << strHeader.readAll(); // copy to output file
    fileIn.close();

    AddHeader(p_bHFImages);
}

void CreatePdf::PrintFooter(bool p_bHFImages, QTextStream& out)
{
    QFile fileIn(":/pdf/IN_NoFooter.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strFooter(&fileIn);
    out << strFooter.readAll();
    fileIn.close();

    AddFooter(p_bHFImages);
    AddPageNumber();
}

void CreatePdf::PrintMenTableTitle(QTextStream& out)
{
    QFile fileIn(":/pdf/IN_MenTableTitle.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintMenTableBody(QTextStream& out)
{
    QList<AthleteData>* pMenList = m_cRank.getAllroundMenList();

    for (int i=0; i<pMenList->size(); i++)
    {
        out << "\t<tr>" << Qt::endl;
        out << "\t  <th rowspan=\"2\" valign=\"middle\">"
            << pMenList->at(i).getRank()
            <<"</th> <!-- Rank -->" << Qt::endl;
        out << "\t  <th rowspan=\"2\" valign=\"middle\" align=\"left\" width=\"150\">"
            << pMenList->at(i).getNameFull()
            << "(" << pMenList->at(i).getNationShort() << ")"
            << "</th>" << Qt::endl;
        out << "\t  <th align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AGTotalScore)
            <<"</th> <!-- Total -->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMFloor)
            <<"</td> <!-- Floor -->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMPommelHorse)
            <<"</td> <!-- Pommel Horse-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMRings)
            <<"</td> <!-- Rings-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMVault)
            <<"</td> <!-- Vault-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMParallelBars)
            <<"</td> <!-- Parallel Bars-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pMenList->at(i).getFinalScore(ApparatusList::AMHorizontalBar)
            <<"</td> <!-- Horizonatl Bar-->" << Qt::endl;
        out << "\t</tr>" << Qt::endl;

        out << "\t<tr>" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AGTotalScore)
            <<")</small></td> <!-- Total -->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMFloor)
            <<")</small></td>  <!-- Floor -->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMPommelHorse)
            <<")</small></td>  <!-- Pommel Horse-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMRings)
            <<")</small></td>  <!-- Rings-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMVault)
            <<")</small></td>  <!-- Vault-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMParallelBars)
            <<")</small></td>  <!-- Parallel Bars-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pMenList->at(i).getDifficultyScore(ApparatusList::AMHorizontalBar)
            <<")</small></td>  <!-- Horizonatl Bar-->" << Qt::endl;
        out << "\t</tr>" << Qt::endl;
    }
}

void CreatePdf::PrintWomenTableTitle(QTextStream& out)
{
    QFile fileIn(":/pdf/IN_WomenTableTitle.html");
    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintWomenTableBody(QTextStream& out)
{
    QList<AthleteData>* pWomenList = m_cRank.getAllroundWomenList();

    for (int i=0; i<pWomenList->size(); i++)
    {
        out << "\t<tr>" << Qt::endl;
        out << "\t  <th rowspan=\"2\" valign=\"middle\">"
            << pWomenList->at(i).getRank()
            <<"</th> <!-- Rank -->" << Qt::endl;
        out << "\t  <th rowspan=\"2\" valign=\"middle\" align=\"left\" width=\"150\">"
            << pWomenList->at(i).getNameFull()
            << "(" << pWomenList->at(i).getNationShort() << ")"
            << "</th>" << Qt::endl;
        out << "\t  <th align=\"center\">"
            << pWomenList->at(i).getFinalScore(ApparatusList::AGTotalScore)
            <<"</th> <!-- Total -->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pWomenList->at(i).getFinalScore(ApparatusList::AWVault)
            <<"</td> <!-- Vault -->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pWomenList->at(i).getFinalScore(ApparatusList::AWUnevenBars)
            <<"</td> <!-- Uneven Bars-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pWomenList->at(i).getFinalScore(ApparatusList::AWBalanceBeam)
            <<"</td> <!-- Balance Beam-->" << Qt::endl;
        out << "\t  <td align=\"center\">"
            << pWomenList->at(i).getFinalScore(ApparatusList::AWFloor)
            <<"</td> <!-- Floor-->" << Qt::endl;
        out << "\t</tr>" << Qt::endl;

        out << "\t<tr>" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pWomenList->at(i).getDifficultyScore(ApparatusList::AGTotalScore)
            <<")</small></td> <!-- Total -->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pWomenList->at(i).getDifficultyScore(ApparatusList::AWVault)
            <<")</small></td>  <!-- Vault -->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pWomenList->at(i).getDifficultyScore(ApparatusList::AWUnevenBars)
            <<")</small></td>  <!-- Uneven Bars-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pWomenList->at(i).getDifficultyScore(ApparatusList::AWBalanceBeam)
            <<")</small></td>  <!-- Balance Beam-->" << Qt::endl;
        out << "\t  <td align=\"center\"><small>("
            << pWomenList->at(i).getDifficultyScore(ApparatusList::AWFloor)
            <<")</small></td>  <!-- Floor-->" << Qt::endl;

        out << "\t</tr>" << Qt::endl;
    }
}

void CreatePdf::PrintSingleMApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusMen p_eApparatus)
{
    QFile fileIn;

    switch (p_eApparatus)
    {
    case ApparatusList::EApparatusMen::AMFloor:
        fileIn.setFileName(":/pdf/IN_Men_Floor.html");
        break;

    case ApparatusList::EApparatusMen::AMHorizontalBar:
        fileIn.setFileName(":/pdf/IN_Men_HorizontalBar.html");
        break;

    case ApparatusList::EApparatusMen::AMParallelBars:
        fileIn.setFileName(":/pdf/IN_Men_ParallelBars.html");
        break;

    case ApparatusList::EApparatusMen::AMPommelHorse:
        fileIn.setFileName(":/pdf/IN_Men_PommelHorse.html");
        break;

    case ApparatusList::EApparatusMen::AMRings:
        fileIn.setFileName(":/pdf/IN_Men_Rings.html");
        break;

    case ApparatusList::EApparatusMen::AMVault:
        fileIn.setFileName(":/pdf/IN_Men_Vault.html");
        break;

    default:
        qDebug() << "Unable to find table title for this apparatus " << p_eApparatus;

    }

    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }
    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintSingleMApparatusTableBody(QTextStream& out)
{
    QList<SingleMWData>* pMenList = m_cRank.getSingleMenList();

    for (int i=0; i<pMenList->size(); i++)
    {
        // Skip if final score is 0
        if (pMenList->at(i).getFinalScore() != "0.000")
        {
            out << "\t<tr>" << Qt::endl;
            out << "\t  <th rowspan=\"2\" valign=\"middle\">"
                << pMenList->at(i).getRank()
                <<"</th> <!-- Rank -->" << Qt::endl;
            out << "\t  <th rowspan=\"2\" valign=\"middle\" align=\"left\" width=\"180\">"
                << pMenList->at(i).getNameFull()
                << "(" << pMenList->at(i).getNation() << ")"
                << "</th>" << Qt::endl;
            out << "\t  <th align=\"center\">"
                << pMenList->at(i).getFinalScore()
                <<"</th> <!-- Single appparatus -->" << Qt::endl;
            out << "\t</tr>" << Qt::endl;

            out << "\t<tr>" << Qt::endl;
            out << "\t  <td align=\"center\"><small>(Difficulty="
                << pMenList->at(i).getDifficultyScore()
                <<", Execution="
                << pMenList->at(i).getExecutionScore()
                <<", Penalty="
                << pMenList->at(i).getPenaltyScore()
                << ")</small></td> <!-- Single appparatus -->" << Qt::endl;
            out << "\t</tr>" << Qt::endl;
        }
    }
}

void CreatePdf::PrintSingleWApparatusTableTitle(QTextStream& out, ApparatusList::EApparatusWomen p_eApparatus)
{
    QFile fileIn;

    switch (p_eApparatus)
    {
    case ApparatusList::EApparatusWomen::AWFloor:
        fileIn.setFileName(":/pdf/IN_Women_Floor.html");
        break;

    case ApparatusList::EApparatusWomen::AWBalanceBeam:
        fileIn.setFileName(":/pdf/IN_Women_BalanceBeam.html");
        break;

    case ApparatusList::EApparatusWomen::AWUnevenBars:
        fileIn.setFileName(":/pdf/IN_Women_UnevenBars.html");
        break;

    case ApparatusList::EApparatusWomen::AWVault:
        fileIn.setFileName(":/pdf/IN_Women_Vault.html");
        break;

    default:
        qDebug() << "Unable to find table title for this apparatus " << p_eApparatus;

    }

    if(!fileIn.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file " << fileIn.errorString();
    }

    QTextStream strTableTitle(&fileIn);
    out << strTableTitle.readAll(); // copy to output file
    fileIn.close();
}

void CreatePdf::PrintSingleWApparatusTableBody(QTextStream& out)
{
    QList<SingleMWData>* pWomenList = m_cRank.getSingleWomenList();

    for (int i=0; i<pWomenList->size(); i++)
    {
        // Skip if final score is 0
        if (pWomenList->at(i).getFinalScore() != "0.000")
        {
            out << "\t<tr>" << Qt::endl;
            out << "\t  <th rowspan=\"2\" valign=\"middle\">"
                << pWomenList->at(i).getRank()
                <<"</th> <!-- Rank -->" << Qt::endl;
            out << "\t  <th rowspan=\"2\" valign=\"middle\" align=\"left\" width=\"180\">"
                << pWomenList->at(i).getNameFull()
                << "(" << pWomenList->at(i).getNation() << ")"
                << "</th>" << Qt::endl;
            out << "\t  <th align=\"center\">"
                << pWomenList->at(i).getFinalScore()
                <<"</th> <!-- Single appparatus -->" << Qt::endl;
            out << "\t</tr>" << Qt::endl;

            out << "\t<tr>" << Qt::endl;
            out << "\t  <td align=\"center\"><small>(Difficulty="
                << pWomenList->at(i).getDifficultyScore()
                <<", Execution="
                << pWomenList->at(i).getExecutionScore()
                <<", Penalty="
                << pWomenList->at(i).getPenaltyScore()
                << ")</small></td> <!-- Single appparatus -->" << Qt::endl;
            out << "\t</tr>" << Qt::endl;
        }
    }
}

void CreatePdf::AddHeader(bool p_bHFImages)
{
    if (p_bHFImages)
    {
        QRectF target(20.0, 10.0, 496.0/3, 239.0/3);
        QRectF source(0.0, 0.0, 496.0, 239.0);
        QImage image(":/pdf/IN_Header_LogoMG.png");
        m_painter.drawImage(target, image, source);  // Paint the mask onto the image
    }
}

void CreatePdf::AddFooter(bool p_bHFImages)
{
    if (p_bHFImages)
    {
        QRectF target(20.0, 750.0, 2480.0/5, 283.0/5);
        QRectF source(0.0, 0.0, 2480.0, 283.0);
        QImage image(":/pdf/IN_Footer_Sponsors.png");
        m_painter.drawImage(target, image, source);  // Paint the mask onto the image
    }
}


void CreatePdf::AddPageNumber()
{
    int x=m_printer.pageRect(QPrinter::Point).width()-60;
    int y=m_printer.pageRect(QPrinter::Point).height()-30;
    m_painter.setFont({"Arial", 10});
    m_painter.setPen({0, 0, 128, 128});
    m_painter.drawText(x, y, "Page " + QString::number(m_iPageNumber));

    // inc for next page
    m_iPageNumber++;
}

void CreatePdf::DrawHtml(const QString& html)
{
    static const QStringList apparatusImageNames = {
        QStringLiteral("BalanceBeam_bw.png"),
        QStringLiteral("Floor_bw.png"),
        QStringLiteral("HorizontalBar_bw.png"),
        QStringLiteral("ParallelBars_bw.png"),
        QStringLiteral("PommelHorse_bw.png"),
        QStringLiteral("Rings_bw.png"),
        QStringLiteral("UnevenBars_bw.png"),
        QStringLiteral("Vault_bw.png")
    };

    QTextDocument doc;
    for (const QString& imageName : apparatusImageNames) {
        const QImage image(QStringLiteral(":/pdf/") + imageName);
        if (image.isNull()) {
            qWarning() << "Unable to load PDF apparatus image" << imageName;
            continue;
        }

        doc.addResource(QTextDocument::ImageResource,
                        QUrl(QStringLiteral("qrc:/pdf/") + imageName),
                        image);
    }

    QString resourceHtml = html;
    resourceHtml.replace(QStringLiteral("../MGServer/pdf/"),
                         QStringLiteral("qrc:/pdf/"));
    doc.setHtml(resourceHtml);
    doc.drawContents(&m_painter);
}

