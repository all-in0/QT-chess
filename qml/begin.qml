import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
Item{
    visible: true
    width: 800
    height: 600
Item {
      id: gameBoard
      x: 0
      y: 0
      width : logic.boardSize * squareSize
      height: logic.boardSize * squareSize

      Image {
        source: "/images/chess_board.jpg"
        height: gameBoard.height
        width: gameBoard.width
      }

      Repeater {
        model: logic

        Image {
          height: squareSize
          width : squareSize

          x: squareSize * positionX
          y: squareSize * positionY

          source: images[type].imgPath

          MouseArea {
            anchors.fill: parent
            drag.target: parent

            property int startX: 0
            property int startY: 0

            onPressed: {
              startX = parent.x;
              startY = parent.y;
            }

            onReleased: {
              var fromX = startX / squareSize;
              var fromY = startY / squareSize;
              var toX   = Math.floor((parent.x + mouseX) / squareSize);
              var toY   = Math.floor((parent.y + mouseY) / squareSize);

              if (!logic.move(fromX, fromY, toX, toY)) {
                parent.x = startX;
                parent.y = startY;
              }
              else{
                  parent.x = squareSize * positionX;
                  parent.y = squareSize * positionY;
              }
            }
          }
        }
      }
}
    Button {
      id: startButton
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10

      text: "New Game"

      onClicked: {
        logic.newGame();
        bg.source = "play.qml"
      }
    }

    Button {
    id: newButton
    anchors.left: gameBoard.right
    anchors.right: parent.right
    anchors.leftMargin: 10
    anchors.rightMargin: 10
    anchors.top: startButton.bottom
    anchors.topMargin: 10

    text: "Load Game"

    onClicked: {
         if (logic.load())
                 bg.source = "load.qml"
    }
    }

//    Button {
//      id: loadButton
//      anchors.left: gameBoard.right
//      anchors.right: parent.right
//      anchors.leftMargin: 10
//      anchors.rightMargin: 10
//      anchors.top: newButton.bottom
//      anchors.topMargin: 10

//      text: "Load Game"

//      onClicked: {
//       if  (logic.load())
//                  source: "/qml/load.qml";
//      }
//    }

//    Button {
//      id: nextButton
//      anchors.left: gameBoard.right
//      anchors.right: parent.right
//      anchors.leftMargin: 10
//      anchors.rightMargin: 10
//      anchors.top: newButton.bottom
//      anchors.topMargin: 70

//      text: "next"

//      onClicked: {
//        logic.nxt();
//      }
//    }
//    Button {
//      id: prevButton
//      anchors.left: gameBoard.right
//      anchors.right: parent.right
//      anchors.leftMargin: 10
//      anchors.rightMargin: 10
//      anchors.top: newButton.bottom
//      anchors.topMargin: 90

//      text: "prev"

//      onClicked: {
//        logic.prv();
//      }
//    }

//     Button {
//      id: saveButton
//      anchors.left: gameBoard.right
//      anchors.right: parent.right
//      anchors.leftMargin: 10
//      anchors.rightMargin: 10
//      anchors.top: newButton.bottom
//      anchors.topMargin: 50

//      text: "Save Game"

//      onClicked: {
//          logic.save();
//      }
//    }

    Button {
      id: exitButton
      anchors.left: gameBoard.right
      anchors.right: parent.right
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      anchors.bottom: gameBoard.bottom
      anchors.bottomMargin: 0

      text: "Exit"

      onClicked: {
        logic.exxx();
      }
    }
}
