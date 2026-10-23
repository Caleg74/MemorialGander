import QtQuick
import QtQuick.Controls
import Memorial.InsertScore

Item {
    id: scoreForm
    objectName: "scoreForm"
    required property ScoreController controller
    width: parent.width
    height: parent.height

    Connections {
        target: scoreForm.controller
        function onMessageRequested(title, text) {
            messageDialog.title = title
            messageDialog.text = text
            messageDialog.open()
        }
    }

    property string finalScore : "0.000"
    readonly property bool gymnastDefined: cbbGymnastSelection.currentIndex > 0 && cbbAppartus.currentIndex > 0
    property int nbrOfReferees : 4
    property bool allowEmptyScore: false

    function resetValues() {
        txtDifficultyScore.text = "0.000"
        txtExecutionScore.text = "0.000"
        txtFinalScore.text = "0.000"
        txtPenaltyScore.text = "0.000"
        txtAvgPenalty.text = "0.000"
        txtPenalty1.text = "0.000"
        txtPenalty2.text = "0.000"
        txtPenalty3.text = "0.000"
        txtPenalty4.text = "0.000"
        txtPenalty1.backColor = "white"
        txtPenalty2.backColor = "white"
        txtPenalty3.backColor = "white"
        txtPenalty4.backColor = "white"
    }

    function loadExistingScore() {
        resetValues()
        if (!gymnastDefined)
            return

        const score = controller.existingScore(cbbGymnastSelection.currentText,
                                               cbbAppartus.currentText)
        if (score.found) {
            txtDifficultyScore.text = score.difficulty
            txtPenaltyScore.text = score.penalty
        }
    }

    function setExclBackground(score)
    {
        let penaltylist2 = [txtPenalty1, txtPenalty2, txtPenalty3, txtPenalty4]

        let i
        for (i=penaltylist2.length-1; i>=0; i--)
        {
            if (parseFloat(penaltylist2[i].text.replace(",", ".")) === parseFloat(score))
            {
                if (Qt.colorEqual(penaltylist2[i].backColor, "#ffffff"))
                {
                    penaltylist2[i].backColor = "#20ff0000"
                    break // exit for loop
                }
            }
        }
    }

    function sortPenaltyScores() {
        let precision = 4;
        // let penaltylist = [parseFloat(txtPenalty1.text.replace(",", ".")),
        //                    parseFloat(txtPenalty2.text.replace(",", ".")),
        //                    parseFloat(txtPenalty3.text.replace(",", ".")),
        //                    parseFloat(txtPenalty4.text.replace(",", "."))]

        let penaltylist1 = [parseFloat(txtPenalty1.text.replace(",", ".")),
                           parseFloat(txtPenalty2.text.replace(",", ".")),
                           parseFloat(txtPenalty3.text.replace(",", ".")),
                           parseFloat(txtPenalty4.text.replace(",", "."))]



        penaltylist1.sort()
        console.log(penaltylist1)

        txtPenalty1.backColor = "white"
        txtPenalty2.backColor = "white"
        txtPenalty3.backColor = "white"
        txtPenalty4.backColor = "white"
        setExclBackground(penaltylist1[0])
        setExclBackground(penaltylist1[3])

        txtAvgPenalty.text = floatToString((penaltylist1[1]+penaltylist1[2])/2, precision)

    }

    function floatToString(num, precision) {
        let str;
        str = num.toFixed(Math.min(precision-1, num.toString().substr(num.toString().indexOf(".")+1).length));
        str = str.slice(0, (str.indexOf(".")) + precision + 1);
        str = str.padEnd(str.indexOf(".") + precision, '0')
        return str;
    }

    function calcFinalScore() {
        let precision = 4;
        if (gymnastDefined)
        {
            if (nbrOfReferees == 2)
            {
                txtAvgPenalty.text = floatToString((parseFloat(txtPenalty1.text.replace(",", "."))
                                + parseFloat(txtPenalty2.text.replace(",", "."))) / 2, precision)
            }
            else if (nbrOfReferees == 4)
            {
                sortPenaltyScores()
            }

            txtExecutionScore.text = floatToString(10.000 - parseFloat(txtAvgPenalty.text), precision)

            let fFinalScore = ( parseFloat(txtDifficultyScore.text.replace(",", "."))
                              + parseFloat(txtExecutionScore.text.replace(",", "."))
                              - parseFloat(txtPenaltyScore.text.replace(",", ".")))

            finalScore = floatToString(fFinalScore, precision);

            //console.log(fFinalScore)

            if (finalScore === "NaN")
            {
                // format error
                finalScore = "Error"
            }
            txtFinalScore.text = finalScore
        }
    }

    function enableSaveEmptyScore(allowEmpty)
    {
        allowEmptyScore = allowEmpty
    }

    function timerCallback() {
        // When closing the settings popup a 60s timer allows to "delete" the score for a selected user
        enableSaveEmptyScore(false)
    }

    Timer {
        id: timer
        interval: 60*1000        // 60s
        running: false
        repeat: false
        onTriggered: scoreForm.timerCallback()
        onTriggeredOnStartChanged: console.log("Timer"+timer.running)
    }

    Rectangle {
        height: parent.height
        width: parent.width
        id: root
        color: "#d0d0d0"
        anchors.fill: parent
        anchors.leftMargin: 20
        opacity: 1
        Column
        {
            width: parent.width
            spacing: 40
            anchors.centerIn: parent

            Row
            {
                spacing: 20
//                anchors.verticalCenter: parent.verticalCenter


                StyleMGComboBox {
                    id: cbbGymnastSelection
                    objectName: "cbbGymnastSelection"
                    model: scoreForm.controller.gymnasts.comboList
                    width: parent.parent.width/2
//                    activeFocusOnPress: true
                    KeyNavigation.tab: cbbAppartus
                    currentIndex: 0
                    onCurrentTextChanged: {
                        scoreForm.controller.selectGymnast(currentText)
                        cbbAppartus.currentIndex = 0
                        scoreForm.resetValues()
                    }
                }
            }

            Row{
                spacing: 20

                StyleMGComboBox {
                    id: cbbAppartus
                    objectName: "apparatusSelection"
                    model: scoreForm.controller.apparatus.comboList
                    emphasizedIndex: scoreForm.controller.finalApparatusIndex
                    width: parent.parent.width/10*4
//                    activeFocusOnPress: true
//                    visible: cbbGymnastSelection.currentIndex > 0
                    KeyNavigation.tab: txtDifficultyScore
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex <= 0)
                            scoreForm.resetValues()
                    }
                    onActivated: scoreForm.loadExistingScore()
                }


                StyleMGCheckbox {
                    id: chkFinalApparatus
                    visible: false
                    text: "Attrezzo Finale"
                }
            }

            Row {
                Column {    // colonna 1
                    spacing: 10
                    Row {
                        spacing: 10
                        Text {
                            width: 120
                            text: "Difficoltà"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtDifficultyScore
                            objectName: "difficultyScore"
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtPenalty1
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }

                        Text {
                            width: 20
                            text: "  "  // just to create a space
                            renderType: Text.NativeRendering
                        }
                    }

                    Row {
                        spacing: 10
                        Text {
                            width: 120
                            text: "+  Esecuzione"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtExecutionScore
                            width: 100
                            activeFocusOnPress: false
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            readOnly: true
                            font.pointSize: 15
                        }
                    }

                    Row {
                        spacing: 10

                        Text {
                            width: 120
                            text: "-  ev. Penalità"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtPenaltyScore
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtDifficultyScore
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }
                    }
                }

                Column {    // colonna 2
                    spacing: 10
                    Row {
                        spacing: 10

                        Text {
                            width: 80
                            text: "Media ded."
                            renderType: Text.NativeRendering
                            font.pointSize: 12
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtAvgPenalty
                            width: parent.parent.width/3
                            activeFocusOnPress: false
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            readOnly: true
                            font.pointSize: 12
                            focus: false
                        }

                        Text {
                            width: 20
                            text: "  "  // just to create a space
                            renderType: Text.NativeRendering
                        }
                    }
                }

                Column {    // colonna 3
                    spacing: 10
                    Row {
                        spacing: 10

                        Text {
                            width: 120
                            text: "Deduzione 1"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtPenalty1
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtPenalty2
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }
                    }

                    Row {
                        spacing: 10

                        Text {
                            width: 120
                            text: "Deduzione 2"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtPenalty2
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtPenalty3
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }
                    }

                    Row {
                        id: rowReferee3
                        spacing: 10

                        Text {
                            width: 120
                            text: "Deduzione 3"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtPenalty3
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtPenalty4
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }
                    }

                    Row {
                        id: rowReferee4
                        spacing: 10

                        Text {
                            width: 120
                            text: "Deduzione 4"
                            renderType: Text.NativeRendering
                            font.pointSize: 15
                            color: "#0a3f60"
                          }

                        StyleMGTextInput {
                            id: txtPenalty4
                            width: 100
                            activeFocusOnPress: true
        //                    style:textEditMGStyle
        //                    placeholderText: "0.000"
                            text: "0.000"
                            font.pointSize: 15
                            KeyNavigation.tab: txtPenaltyScore
                            onTextChanged: scoreForm.calcFinalScore()
                            onFocusChanged:{
                                if(focus)
                                   selectAll()
                            }
                        }
                    }
                }
            }

            Row {
                spacing: 20

                StyleMGTextInput {
                    id: txtFinalScore
                    width: parent.parent.width/5
                    activeFocusOnPress: false
                    readOnly: true
                    text: scoreForm.finalScore
                    font.pointSize: 15
                    font.bold: true
                }

                StyleMGPushButton {
                    id:btnSaveScore
                    objectName: "btnSaveScore"
                    width: 110
                    height: 40
                    buttonText: "Registra"

                    enabled: ((parseFloat(txtDifficultyScore.text.replace(",", ".")) > 0.0) || scoreForm.allowEmptyScore)
                             && (cbbGymnastSelection.currentIndex > 0)
                             && (cbbAppartus.currentIndex > 0)
                    onClicked: {
                        const saved = scoreForm.controller.saveScore(cbbGymnastSelection.currentText,
                                  cbbAppartus.currentText,
                                  txtDifficultyScore.text,
                                  txtExecutionScore.text,
                                  txtPenaltyScore.text,
                                  txtFinalScore.text,
                                  chkFinalApparatus.checked)

                        if (saved) {
                            scoreForm.resetValues()
                            cbbAppartus.currentIndex = 0
                        }


                    }
                }

            }
        }

        //************************************************************************************

        Popup {
            id: popupSettings
            objectName: "popupSettings"
            x: root.width-btnSettings.width-width-5
            y: btnSettings.height
            width: 270
            height: 190
            modal: true
            focus: true
            spacing: 10

            background: Rectangle {
                    color: "#e0e0e0"
                    border.color: "darkgrey"
                }
            enter: Transition {
                   NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 300 }
               }
            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 300 }
            }
            Column {
                spacing: 10
                Row {
                    Text {
                        text: "Impostazioni:"
                        renderType: Text.NativeRendering
                        font.pointSize: 12
                        font.bold: true
                        color: "#0a3f60"
                      }
                }

                Row {
                    Text {
                        width: 130
                        text: "Numero giurati"
                        renderType: Text.NativeRendering
                        font.pointSize: 12
                        color: "#0a3f60"
                      }

                    StyleMGComboBox {
                        id: cbbRefereeNbr
                        objectName: "cbbRefereeNbr"
                        width: 40
                        font.pointSize: 12
                        model: ["2", "4"]
        //                    activeFocusOnPress: true
                        currentIndex: scoreForm.nbrOfReferees === 2 ? 0 : 1
                    }
                }

                Row {
                    Text {
                        width: 130
                        text: "Lista ginnastisti"
                        renderType: Text.NativeRendering
                        font.pointSize: 12
                        color: "#0a3f60"
                      }

                    StyleMGCheckbox {
                        id: chkListM
                        objectName: "categoryMen"
                        checked: true
                        text: "M"
                    }
                    StyleMGCheckbox {
                        id: chkListF
                        objectName: "categoryWomen"
                        checked: true
                        text: "F"
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    StyleMGPushButton {
                        id:btnSaveSettings
                        width: 70
                        height: 40
                        buttonText: "Chiudi"
                        onClicked: {
                            popupSettings.close()
                        }
                    }
                }
            }
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
            onOpened: { root.opacity = 0.5}
            onClosed: {
                root.opacity = 1
                if (cbbRefereeNbr.currentIndex === 0) // = 2 referrees
                {
                    scoreForm.nbrOfReferees = 2;
                    rowReferee3.visible = false;
                    rowReferee4.visible = false;
                }
                else if (cbbRefereeNbr.currentIndex === 1) // = 4 referres
                {
                    scoreForm.nbrOfReferees = 4;
                    rowReferee3.visible = true;
                    rowReferee4.visible = true;
                }

                scoreForm.resetValues()

                scoreForm.controller.setCategories(chkListM.checked, chkListF.checked)

                scoreForm.enableSaveEmptyScore(true)
                timer.start()
            }
        }

        StyleMGMsgBox {
            id: messageDialog
            visible: false
        }
        Button {
            id: btnSettings
            anchors.right: parent.right
            anchors.top: parent.top
            text: qsTr("...")
            font.bold: true
            font.pointSize: 18

            background: Rectangle {
                    color: btnSettings.down ? "#bbbbbb" :
                            (btnSettings.hovered ? "#d6d6d6" : "transparent")
            }
            onClicked: popupSettings.open()
        }
    }
}

