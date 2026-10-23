#include "createpdf.h"
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QHash>
#include "dbinterface.h"


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
            strFileName += "/MemorialGander2026.pdf";
        else
            strFileName += "/MemorialGander2026_NoImage.pdf";

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
    m_cRank.updateAllroundScores();

    const QString strFileName = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
                              + "/MemorialGander2026_FinalApparatusStartingOrder.pdf";

    m_printer.setOutputFormat(QPrinter::PdfFormat);
    m_printer.setPageSize(QPageSize::A4);
    m_printer.setOutputFileName(strFileName);

    if (! m_painter.begin(&m_printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return;
    }

    m_iPageNumber = 1;
    PrintLastApparatusOrder(true);

    m_painter.end();

    qDebug() << "File created in " << strFileName;
}

void CreatePdf::PrintLastApparatusOrder(bool p_bHFImages)
{
    QString html;
    QTextStream out(&html);

    PrintHeader(p_bHFImages, out);

    QList<QStringList> subscriptions;
    dbInterface::Instance()->retrieveGymnastSubscriptionList(subscriptions);
    QHash<int, int> assignedApparatusByAthlete;
    for (const QStringList &subscription : subscriptions) {
        if (subscription.size() >= 7)
            assignedApparatusByAthlete.insert(subscription.at(0).toInt(), subscription.at(6).toInt());
    }

    const auto printStartingOrder = [&](bool men, QList<AthleteData> *rankedGymnasts,
                                        bool closeTable) {
        QFile titleFile(men ? ":/pdf/IN_LastApparatusMenTitle.html"
                            : ":/pdf/IN_LastApparatusWomenTitle.html");
        if (!titleFile.open(QIODevice::ReadOnly)) {
            qCritical() << "Unable to open final apparatus title template:" << titleFile.errorString();
            return;
        }
        QTextStream titleStream(&titleFile);
        out << titleStream.readAll();
        titleFile.close();

        const int gymnastCount = qMin(6, rankedGymnasts->size());
        const QString gender = men ? QStringLiteral("M") : QStringLiteral("F");
        for (int order = 0; order < gymnastCount; ++order) {
            AthleteData gymnast = rankedGymnasts->at(gymnastCount - order - 1);
            const int apparatusId = assignedApparatusByAthlete.value(gymnast.getAthleteId(), 0);
            const QString apparatus = ApparatusList::Instance()->apparatusName(apparatusId, ApparatusData::ALangEn);

            out << "<tr><td align=\"center\">" << (order + 1) << "</td>"
                << "<td>" << gymnast.getNameFull().trimmed().toHtmlEscaped() << "</td>"
                << "<td>";
            if (!apparatus.isEmpty()
                && ApparatusList::Instance()->getApparatusId(apparatus, gender) == apparatusId)
                out << apparatus.toHtmlEscaped();
            out << "</td></tr>" << Qt::endl;
        }
        if (closeTable)
            out << "</table><br/>" << Qt::endl;
    };

    printStartingOrder(true, m_cRank.getAllroundMenList(), true);
    printStartingOrder(false, m_cRank.getAllroundWomenList(), false);
    PrintFooter(p_bHFImages, out);

    out.flush();
    DrawHtml(html);
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
    DrawAllroundHtml(html);
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
    DrawAllroundHtml(html);
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
    PrintAllroundTableBody(out, *m_cRank.getAllroundMenList(), ApparatusList::AMApparatusMax);
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
    PrintAllroundTableBody(out, *m_cRank.getAllroundWomenList(), ApparatusList::AWApparatusMax);
}

void CreatePdf::PrintAllroundTableBody(QTextStream& out, const QList<AthleteData>& gymnasts,
                                     int apparatusEnd)
{
    const auto numericScore = [](const QString &score) {
        const QString value = score.trimmed();
        return value == QStringLiteral("-") ? QStringLiteral("0.000") : value;
    };
    for (const auto &gymnast : gymnasts) {
        out << "<tr><td align=\"center\" style=\"font-size:10px;font-weight:bold;\">"
            << gymnast.getRank() << "</td>"
            << "<td style=\"font-size:10px;font-weight:bold;\">"
            << gymnast.getNameFull().trimmed().toHtmlEscaped()
            << " (" << gymnast.getNationShort().toHtmlEscaped() << ")</td>";

        // Total first, followed by each apparatus in the existing column order.
        for (int apparatus = ApparatusList::AGTotalScore; apparatus < apparatusEnd; ++apparatus) {
            const QString final = gymnast.getFinalScore(apparatus).trimmed();
            out << "<td align=\"center\"><span style=\"font-size:11px;font-weight:bold;\">"
                << final << "</span>";
            if (final != QStringLiteral("-")) {
                out << "<br/><span style=\"font-size:8px;font-weight:normal;\">"
                    << "D " << numericScore(gymnast.getDifficultyScore(apparatus))
                    << "<br/>E " << numericScore(gymnast.getExecutionScore(apparatus))
                    << "</span>";
            }
            out << "</td>";
        }
        out << "</tr>" << Qt::endl;
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
            out << "\t  <td align=\"center\"><small>(D="
                << pMenList->at(i).getDifficultyScore()
                <<", E="
                << pMenList->at(i).getExecutionScore()
                <<", P="
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
            out << "\t  <td align=\"center\"><small>(D="
                << pWomenList->at(i).getDifficultyScore()
                <<", E="
                << pWomenList->at(i).getExecutionScore()
                <<", P="
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
        QRectF target(20.0, 750.0, 2480.0/3, 283.0/3);
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

void CreatePdf::DrawAllroundHtml(QString html)
{
    // The legacy <font size> wrapper overrides nested CSS font sizes in Qt.
    // Remove it only on the two overall pages, which define their own typography.
    html.remove(QStringLiteral("<font face=\"arial\" size=\"2\">"));
    html.remove(QStringLiteral("</font>"));
    DrawHtml(html);
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
