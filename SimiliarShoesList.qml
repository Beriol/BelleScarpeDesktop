import QtQuick 2.0

Rectangle
{
    id: container

    /**************************************************************
     * Costanti usate per definire le grandezze dei vari elementi
     **************************************************************/

    //Dimensioni di ogni thumbnail della lista
    property real listEntryWidth: 230 * scaleX
    property real listEntryHeight: 230 * scaleY
    property real listEntryHalvedHeight: listEntryHeight / 2

    //Larghezza totale della lista (tiene conto della larghezza della scrollbar)
    property real listContainerWidth: (listEntryWidth + 30 * scaleX)


    height: parent.height
    width: 500 * scaleX

    FontLoader { id: metroFont; source: "qrc:segeo-wp.ttf" }

    ListView {
        id: similarList

        //La lista è grande quanto tutto lo schermo, quindi "riempie" tutto il parent
        anchors.fill: parent


        /* Segnalo che la lista non deve seguire automaticamente l'elemento attualmente selezionato; senza questo booleano
         * la lista si sposterebbe da sola (moooolto lentamente) verso l'elemento da visualizzare quando la lista diventa
         * inizialmente visibile */
        highlightFollowsCurrentItem: false

        //Il modello della lista, contenente i path delle immagini da mostrare, è preso da C++ ed è uguale a quello della lista
        //contenente le thumbnail
        model: similiarShoesModel

        //Il delegate corrisponde ad una singola immagine per ogni item della lista
        delegate: Component {

            Rectangle {
                id: suggestionContainer
                width: similarList.width
                height: 190 * scaleY
                color: "#00000000"

                Image {
                    id: similarThumbnail
                    antialiasing: true
                    source: modelData.thumbnail
                    width: 200 * scaleX
                    height: 170 * scaleY
                    fillMode: Image.PreserveAspectFit
                    anchors.right: suggestionContainer.right
                    anchors.rightMargin: 5 * scaleX
                    anchors.verticalCenter: suggestionContainer.verticalCenter
                }

                Text {
                    id: t1
                    anchors.left: suggestionContainer.left
//                    anchors.leftMargin: 50 * scaleX
                    font.letterSpacing: 1.2
                    color: "#9FB7BF"
                    text: modelData.brand + " " + modelData.model
                    font.family: metroFont.name
                    font.pointSize: 16
                    font.weight: Font.Light
                }

                Text {
                    id: brand
                    anchors.top: t1.bottom
                    anchors.topMargin: 15 * scaleY
                    text: modelData.brand
                    font.family: metroFont.name
                    font.pointSize: 14
                    font.weight: Font.Light
                }

                Text {
                    id: model
                    anchors.top: brand.bottom
                    anchors.topMargin: 15 * scaleY
                    text: modelData.model
                    font.family: metroFont.name
                    font.pointSize: 14
                    font.weight: Font.Light
                }

                Text {
                    id: price
                    anchors.top: model.bottom
                    anchors.topMargin: 15 * scaleY
                    text: modelData.price
                    font.family: metroFont.name
                    font.pointSize: 14
                    font.weight: Font.Light
                }

                Rectangle {
                    id: separator
                    width: parent.width
                    height: 1 * scaleY
                    color: "#9FB7BF"
                    anchors.bottom: suggestionContainer.bottom
                }

            }
        }

        orientation: ListView.Vertical
        spacing: 20
    }
}