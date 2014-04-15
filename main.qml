import QtQuick 2.0

Rectangle {
    id: mainWindow
    width: 1920 * scaleX
    height: 1080 * scaleY


    //I figli del ViewManager sono gestiti dallo stesso. Uno solo dei figli può essere visibili in un dato momento
    ViewManager {
        id: myViewManager
        objectName: "myViewManager"


        Rectangle {
            id: view1
            color: "black"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#000000";
                }
                GradientStop {
                    position: 0.61;
                    color: "#ffffff";
                }
                GradientStop {
                    position: 0.64;
                    color: "#ffffff";
                }
            }
            width:  parent.width
            height: parent.height

            Text {
                id: view1Text
                text: "Wait for it"
                font.bold: true
                styleColor: "#000000"
                font.pointSize: 13
                color: "#222121"
                anchors.centerIn: parent
            }

            Rectangle {
                id: backButton
                x: 30
                y: 30
                width: 200 * scaleX
                height: 100 * scaleY
                border.width: 3
                radius: 5
                color: "green";

                Text {
                    text: "back"
                    color: "black"
                    anchors.centerIn: parent
                }

                MouseArea {
                     anchors.fill: parent
                     onClicked: {
                         myViewManager.goBack();
                     }
                 }
            }

            Rectangle {
                anchors.left: backButton.right
                anchors.verticalCenter: backButton.verticalCenter
                width: 200 * scaleX
                height: 100 * scaleY
                border.width: 3
                radius: 5
                color: "lightgreen";

                Text {
                    text: "next"
                    color: "black"
                    anchors.centerIn: parent
                }

                MouseArea {
                     anchors.fill: parent
                     onClicked: {
                         mainWindow.addView();
                     }
                 }
            }
        }

    }

    /* Funzione per aggiungere una view al ViewManager dinamicamente (keyword da cercare su google: Dynamic QML Object Creation from JavaScript).
     * Dopo l'esecuzione del metodo, la nuova view creata diventerà la view visibile.
     * Questa funzione è chiamata da c++ usando il metodo invokeMethod(); è messa quindi in questo file in modo che ci si possa accedere facilmente */
    function addView()
    {
        //Creo il componente; la view deve essere quindi definita in un file a parte, e verrà usata come custom component di qml
        var component = Qt.createComponent("ShoeView.qml");

        //Preso il component, creo una sua istanza e passo come parent il ViewManager, in modo che la nuova view diventi sua figlia
        var newView = component.createObject(myViewManager);

        //Controllo che l'oggetto sia stato creato correttamente
        if(newView == null)
        {
            console.log("C'è stato un errore nell'aggiunta della nuova view");
            return;
        }

        //Setto inizialmente la visibilità della nuova view su falso
        newView.visible = false;

        //Connetto la visibilità della view con il metodo per gestire i cambi di view
        myViewManager.connectViewEvents(newView);

        //Adesso che la view è connessa col gestore, la rendo visibile. Questo farà si che la view corrente sparisca per lasciare
        //spazio alla view appena aggiunta
        newView.visible = true;
    }

}
