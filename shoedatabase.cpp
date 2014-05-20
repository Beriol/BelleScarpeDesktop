#include <shoedatabase.h>
#include <shoe.h>
#include <QString>
#include <QDebug>
#include <QQmlContext>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <vector>
#include <QDir>


//Namespace contenente classi come "vector" e "map". Usare il namespace fa si che non si debba scrivere ad esempio std::vector quando lo si usa
using namespace std;


//Costanti per accedere al database
const QString ShoeDatabase::HOST_NAME = "localhost";
const int ShoeDatabase::DB_PORT = 3306;
const QString ShoeDatabase::DB_NAME = "my_bellescarpecod";
const QString ShoeDatabase::DB_USERNAME = "root";
const QString ShoeDatabase::DB_PASSWORD = "";

//Nomi delle tabelle
const QString ShoeDatabase::SHOE_TABLE_NAME = "scarpa";
const QString ShoeDatabase::SIZE_TABLE_NAME = "taglia";


//Nomi delle colonne della tabella delle scarpe
const QString ShoeDatabase::SHOE_ID_COLUMN = "id_scarpa";
const QString ShoeDatabase::SHOE_BRAND_COLUMN = "marca";
const QString ShoeDatabase::SHOE_MODEL_COLUMN = "modello";
const QString ShoeDatabase::SHOE_COLOR_COLUMN = "colore";
const QString ShoeDatabase::SHOE_SEX_COLUMN = "sesso";
const QString ShoeDatabase::SHOE_CATEGORY_COLUMN = "categoria";
const QString ShoeDatabase::SHOE_PRICE_COLUMN = "prezzo";
const QString ShoeDatabase::SHOE_MEDIA_COLUMN = "media";
const QString ShoeDatabase::SHOE_RFID_CODE_COLUMN = "rfid_code";


//Posizioni delle colonne della tabella delle scarpe
const int ShoeDatabase::SHOE_ID_COLUMN_POSITION = 0;
const int ShoeDatabase::SHOE_BRAND_COLUMN_POSITION = 1;
const int ShoeDatabase::SHOE_MODEL_COLUMN_POSITION = 2;
const int ShoeDatabase::SHOE_COLOR_COLUMN_POSITION = 3;
const int ShoeDatabase::SHOE_SEX_COLUMN_POSITION = 4;
const int ShoeDatabase::SHOE_CATEGORY_COLUMN_POSITION = 5;
const int ShoeDatabase::SHOE_PRICE_COLUMN_POSITION = 6;
const int ShoeDatabase::SHOE_MEDIA_COLUMN_POSITION = 7;
const int ShoeDatabase::SHOE_RFID_CODE_COLUMN_POSITION = 8;



//Nomi delle colonne della tabella delle taglie
const QString ShoeDatabase::SIZE_ID_COLUMN = "id_scarpa";
const QString ShoeDatabase::SIZE_SIZE_COLUMN = "taglia";
const QString ShoeDatabase::SIZE_QUANTITY_COLUMN = "quantita";


//Posizioni delle colonne della tabella delle taglie
const int ShoeDatabase::SIZE_ID_COLUMN_POSITION = 0;
const int ShoeDatabase::SIZE_SIZE_COLUMN_POSITION = 1;
const int ShoeDatabase::SIZE_QUANTITY_COLUMN_POSITION = 2;


/**
 * @brief ShoeDatabase::ShoeDatabase costruttore; istanzia il database
 */
ShoeDatabase::ShoeDatabase()
{
    //Setup del database
    db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(ShoeDatabase::HOST_NAME);
    db.setPort(ShoeDatabase::DB_PORT);
    db.setDatabaseName(ShoeDatabase::DB_NAME);
    db.setUserName(ShoeDatabase::DB_USERNAME);
    db.setPassword(ShoeDatabase::DB_PASSWORD);
}

/**
 * @brief ShoeDatabase::open apre il db
 *
 * @return true se è stato aperto, false altrimenti
 */
bool ShoeDatabase::open()
{
    db.open();

    if(!db.isOpen())
    {
        qDebug() << db.lastError();

        return false;
    }

    return true;
}


/**
 * @brief ShoeDatabase::getShoeFromId preleva i dati della scarpa con l'id specificato
 *
 * @param shoeId l'id della scarpa da cercare
 *
 * @return un oggetto Shoe contenente i dati (è un puntatore perchè estende QObject e gli oggetti che estendono QObject non possono
 *         essere copiati, bisogna passare per forza un riferimento). Viene ritornato NULL in caso di errori o se non è stato trovato niente
 */
Shoe* ShoeDatabase::getShoeFromId(int shoeId)
{
    //Preparo la query vera e propria da eseguire in base all'id passato
    QString queryString = "SELECT * FROM " + ShoeDatabase::SHOE_TABLE_NAME + " " +
                          "WHERE " + ShoeDatabase::SHOE_ID_COLUMN + " = " + QString::number(shoeId);

    //Chiamo il metodo che si occuperà di eseguire la query vera e propria e di restituire la scarpa trovata
    return getShoe(queryString);
}


/**
 * @brief ShoeDatabase::getShoeFromId preleva i dati della scarpa con l'id specificato
 *
 * @param RFIDcode il codice RFID associato alla scarpa da cercare
 *
 * @return un oggetto Shoe contenente i dati (è un puntatore perchè estende QObject e gli oggetti che estendono QObject non possono
 *         essere copiati, bisogna passare per forza un riferimento). Viene ritornato NULL in caso di errori o se non è stato trovato niente
 */
Shoe* ShoeDatabase::getShoeFromId(QString RFIDcode)
{
    //Preparo la query vera e propria da eseguire in base al codice RFID passato
    QString queryString = "SELECT * FROM " + ShoeDatabase::SHOE_TABLE_NAME + " "+
                          "WHERE " + ShoeDatabase::SHOE_RFID_CODE_COLUMN + " = " + "'" + RFIDcode + "'";

    //Chiamo il metodo che si occuperà di eseguire la query vera e propria e di restituire la scarpa trovata
    return getShoe(queryString);
}


/**
 * @brief ShoeDatabase::getShoe preleva i dati della scarpa dopo aver eseguito la query passatagli come stringa
 *
 * @param queryString la query sotto forma di stringa da usare per il recupero
 *
 * @return un oggetto Shoe contenente i dati (è un puntatore perchè estende QObject e gli oggetti che estendono QObject non possono
 *         essere copiati, bisogna passare per forza un riferimento). Viene ritornato NULL in caso di errori o se non è stato trovato niente
 */
Shoe* ShoeDatabase::getShoe(QString queryString)
{
    QSqlQuery query;

    //Eseguo la query
    query.exec(queryString);


    //Se non ci sono stati errori e se è stata trovata esattamente una scarpa procedo
    if(query.lastError().number() == -1 && query.size() == 1)
    {
        //Prelevo il primo (e unico) risultato
        query.next();

        //Recupero tutti gli elementi; sono ordinati in base a come sono stati specificati nella SELECT. In questo caso sono nell'ordine
        //con cui sono state messe le colonne della tabella, quindi ci accedo con le costanti specificate in questa classe
        int shoeId = query.value(ShoeDatabase::SHOE_ID_COLUMN_POSITION).toInt();
        QString brand = query.value(ShoeDatabase::SHOE_BRAND_COLUMN_POSITION).toString();
        QString model = query.value(ShoeDatabase::SHOE_MODEL_COLUMN_POSITION).toString();
        QString color = query.value(ShoeDatabase::SHOE_COLOR_COLUMN_POSITION).toString();
        QString sex = query.value(ShoeDatabase::SHOE_SEX_COLUMN_POSITION).toString();
        QString category = query.value(ShoeDatabase::SHOE_CATEGORY_COLUMN_POSITION).toString();
        float price = query.value(ShoeDatabase::SHOE_PRICE_COLUMN_POSITION).toFloat();
        QString mediaPath = query.value(ShoeDatabase::SHOE_MEDIA_COLUMN_POSITION).toString();
        QString RFIDcode = query.value(ShoeDatabase::SHOE_RFID_CODE_COLUMN_POSITION).toString();



        //Adesso devo recuperare tutte le taglie disponibili per la scarpa. Per farlo devo eseguire un'altra query
        QSqlQuery queryForSizes;

        //Eseguo la query sulla tabella delle taglie
        queryForSizes.exec("SELECT * FROM " + ShoeDatabase::SIZE_TABLE_NAME + " WHERE " + ShoeDatabase::SIZE_ID_COLUMN + " = " + QString::number(shoeId) + " ORDER BY " + ShoeDatabase::SIZE_SIZE_COLUMN);

        QVariantMap sizesAndQuantities;

        //Se non ci sono stati errori, procedo
        if(queryForSizes.lastError().number() == -1)
        {
            //Scorro tutti i risultati, fino a quando non ce ne sono più
            while(queryForSizes.next())
            {
                float size = queryForSizes.value(ShoeDatabase::SIZE_SIZE_COLUMN_POSITION).toFloat();
                int quantity = queryForSizes.value(ShoeDatabase::SIZE_QUANTITY_COLUMN_POSITION).toInt();

                QString sizeString = QString::number(size);

                //Inserisco nell'array associativo la taglia ed il booleano che segnala se è disponibile o meno
                sizesAndQuantities[sizeString] = quantity > 0;
            }
        }

        //Aggiunge taglie manualmente per testare la visualizzazione su schermo
//        for(int k = 48; k < 52; k++){
//            sizesAndQuantities[QString::number(k)] = 23;
//        }

        //Ora che ho tutti i dati, creo l'oggetto Shoe...
        Shoe* shoe = new Shoe(shoeId, brand, model, color, sex, price, category, sizesAndQuantities, mediaPath, RFIDcode);

        //...e lo ritorno
        return shoe;
    }
    else
    {
        if(query.size() == 0)
            qDebug() << "ShoeDatabase::getShoe: nessuna scarpa è stata trovata";
        else
            qDebug() << "ShoeDatabase::getShoe: c'è stato un errore nella query: " << query.lastError();

//        qDebug() << query.lastError();

        return NULL;
    }
}


/**
 * @brief ShoeDatabase::getSimiliarShoes
 *        preleva le scarpe che hanno proprietà simili alla scarpa con id specificato, in base ai parametri passati
 *
 * @param shoeId id della scarpa che si deve scartare
 * @param sex sesso della scarpa
 * @param category categoria della scarpa
 *
 * @return la lista delle scarpe simili; NOTA: le scarpe avranno l'array contenente le taglie e le quantità vuoto
 */
vector<Shoe*> ShoeDatabase::getSimiliarShoes(int shoeId, QString sex, QString category)
{
    QSqlQuery query;


    query.exec("SELECT * FROM " + ShoeDatabase::SHOE_TABLE_NAME +
               " WHERE " + ShoeDatabase::SHOE_ID_COLUMN + " != " + QString::number(shoeId) +
               " AND " + ShoeDatabase::SHOE_SEX_COLUMN + " = '" + sex + "' "
               " AND " + ShoeDatabase::SHOE_CATEGORY_COLUMN + " = '" + category + "'");

    vector<Shoe*> shoeList;

    if(query.lastError().number() == -1 && query.size() > 0)
    {
        while(query.next())
        {
            int id = query.value(ShoeDatabase::SHOE_ID_COLUMN_POSITION).toInt();
            QString brand = query.value(ShoeDatabase::SHOE_BRAND_COLUMN_POSITION).toString();
            QString model = query.value(ShoeDatabase::SHOE_MODEL_COLUMN_POSITION).toString();
            QString color = query.value(ShoeDatabase::SHOE_COLOR_COLUMN_POSITION).toString();
            QString sex = query.value(ShoeDatabase::SHOE_SEX_COLUMN_POSITION).toString();
            QString category = query.value(ShoeDatabase::SHOE_CATEGORY_COLUMN_POSITION).toString();
            float price = query.value(ShoeDatabase::SHOE_PRICE_COLUMN_POSITION).toFloat();
            QString mediaPath = query.value(ShoeDatabase::SHOE_MEDIA_COLUMN_POSITION).toString();
            QString RFIDcode = query.value(ShoeDatabase::SHOE_RFID_CODE_COLUMN_POSITION).toString();


            //Array vuoto da inserire nel costruttore; è vuoto perchè per visualizzare le scarpe simili non importa sapere le taglie
            QVariantMap sizesAndQuantities;

            //Creo la nuova scarpa
            Shoe *shoe = new Shoe(id, brand, model, color, sex, price, category, sizesAndQuantities, mediaPath, RFIDcode);


            //Dato che le scarpe consigliate hanno una thumbnail, devo settare il path all'immagine per ogni scarpa. Prendo
            //quindi il path assoluto della cartella che conterrà la thumbnail
            QDir path = QDir::currentPath() + "/debug/shoes_media/" + shoe->getMediaPath() + "/thumbnail/";


            //Filtro per recuperare solo immagini, non si sa mai
            QStringList nameFilter;
            nameFilter << "*.png" << "*.jpg" << "*.gif";

            //Recupero il path del primo file trovato che soddisfi i filtri; userò quello come thumbnail
            QString thumbnailPath = "file:///" + path.entryInfoList(nameFilter, QDir::Files, QDir::Name).first().absoluteFilePath();

            //Setto quindi il path trovato come thumbnail della scarpa
            shoe->setThumbnailPath(thumbnailPath);

            //Infine, inserisco la scarpa nell'array
            shoeList.push_back(shoe);
        }

        return shoeList;
    }
    else
    {
        if(query.size() == 0)
            qDebug() << "ShoeDatabase::getSimiliarShoes: nessuna scarpa simile è stata trovata; id scarpa:" << shoeId;
        else
            qDebug() << "ShoeDatabase::getSimiliarShoes: c'è stato un errore nella query: " << query.lastError();

        //Restituisco l'array vuoto
        return shoeList;
    }
}



/**
 * @brief ShoeDatabase::close chiude il db
 */
void ShoeDatabase::close()
{
    db.close();
}
