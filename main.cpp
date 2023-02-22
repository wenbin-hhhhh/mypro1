#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>

void writeJsonToFile(char *fileName)
{
    //第一种: 将json对象写入磁盘文件
    //创建一个json对象
    /*QJsonObject json;

    //给json对象插入值
    // iterator insert(const QString &key, const QJsonValue &value)
    json.insert("name", "xiaowu");
    json.insert("age", 21);
    json.insert("sex", "female");

    //插入子对象
    QJsonObject subJson;
    subJson.insert("father", "longji");
    subJson.insert("mather", "liwei");
    subJson.insert("sister", "wangjin");
    json.insert("family", subJson);

    //插入json数组
    QJsonArray jsonArr;
    jsonArr.append("english");
    jsonArr.append("chinese");
    jsonArr.append("math");
    jsonArr.append("history");
    json.insert("course", jsonArr);*/

    //第二种: 将json数组写入磁盘文件
    //构造一个QJsonArray对象
    QJsonArray array;

    //void append(const QJsonValue &value)
    //给array对象添加值
    array.append(10);
    array.append("jinyanlong");
    array.append(true);

    //添加子数组到array中
    QJsonArray subArray;
    subArray.append("english");
    subArray.append("chinese");
    subArray.append("history");
    array.append(subArray);

    //添加对象到array中
    QJsonObject subObj;
    subObj.insert("mother", "xiaoling");
    subObj.insert("father", "xiaotong");
    subObj.insert("sister", "damiao");
    array.append(subObj);

    //将QJsonObject 转换为 QJsonDocument
    //JsonDocument::QJsonDocument(const QJsonObject &object)
    //QJsonDocument jsonDoc(json);

    //QJsonDocument(const QJsonArray &array)
    QJsonDocument jsonDoc(array);

    //将QJsonDocument对象转换为QByteArray对象
    //QByteArray toJson(JsonFormat format = Indented) const
    QByteArray byteArray = jsonDoc.toJson();

    //文件操作--将byteArray写入文件
    //QFile(const QString &name)
    QFile file(fileName);

    //打开文件
    //virtual bool open(OpenMode mode)
    file.open(QIODevice::WriteOnly);

    //写文件
    //qint64 write(const QByteArray &byteArray)
    file.write(byteArray);

    //关闭文件
    //virtual void close()
    file.close();

    return;
}

void readJsonFromFile(char *fileName)
{
    //构造QFile类对象
    QFile file;

    //设置要读的文件
    //void setFileName(const QString &name)
    file.setFileName(fileName);

    //打开文件
    //virtual bool open(OpenMode mode)
    file.open(QIODevice::ReadOnly);

    //读文件
    //QByteArray readAll()
    QByteArray byteArray = file.readAll();

    //关闭文件
    //virtual void close()
    file.close();

    //将QByteArray类对象转换为QJsonDocument
    //QJsonDocument fromJson(const QByteArray &json, QJsonParseError *error = Q_NULLPTR)
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);

    //判断是数组还是对象
    if(jsonDoc.isObject())
    {
        //QJsonObject object() const
        QJsonObject jsonObj = jsonDoc.object();

        //获取对象中所有的key值
        //QStringList keys() const
        QStringList keys = jsonObj.keys();

        for(int i=0; i<keys.size(); i++)
        {
            //获取每一个key值
            //QString key = keys[i];
            QString key = keys.at(i);
            //qDebug() << key << ":";

            //根据key值获取value值
            //QJsonValue value(const QString &key) const
            //QJsonValue operator[](const QString &key) const
            QJsonValue jsonValue = jsonObj.value(key);

            //判断value值的类型
            //bool isString() const
            if(jsonValue.isString())
            {
                qDebug() <<  key << ":" << jsonValue.toString();
            }
            else if(jsonValue.isDouble())
            {
                qDebug() <<  key << ":" << jsonValue.toInt();
            }
            else if(jsonValue.isBool())
            {
                qDebug() <<  key << ":" << jsonValue.toBool();
            }
            else if(jsonValue.isObject())
            {
                //QJsonObject toObject() const
                 QJsonObject obj = jsonValue.toObject();
                 QStringList subKeys = obj.keys();

                 qDebug() << key << ":{";
                 for(int k=0; k<subKeys.size(); k++)
                 {
                     QString subkey = subKeys[k];
                     QJsonValue subJsonValue = obj.value(subkey);
                     qDebug() << "  " << subJsonValue.toString();
                 }
                 qDebug() << "}";
            }
            else if(jsonValue.isArray())
            {
               //QJsonArray toArray() const
               qDebug() << key << ":[";
               QJsonArray arr =  jsonValue.toArray();
               for(int j=0; j<arr.size(); j++)
               {
                   QJsonValue va = arr[j];
                   if(va.isString())
                   {
                       qDebug() << "    " << va.toString();
                   }
               }
               qDebug() <<"]";

            }

        }

    }
    else if(jsonDoc.isArray())
    {
        //QJsonArray array() const
        QJsonArray array = jsonDoc.array();
        for(int i=0; i<array.size(); i++)
        {
            QJsonValue value = array[i];

            //判断值的类型
            if(value.isString())
            {
                qDebug() << value.toString();
            }
            else if(value.isDouble())
            {
                qDebug() << value.toInt();
            }
            else if(value.isBool())
            {
                qDebug() << value.toBool();
            }
            else if(value.isArray())
            {
                qDebug() << "[";
                QJsonArray subArray = value.toArray();
                for(int j=0; j<subArray.size(); j++)
                {
                    qDebug() << "   " << subArray[j].toString();
                }
                qDebug() << "]";
            }
            else if(value.isObject())
            {
                qDebug() << "{";
                QJsonObject subObj = value.toObject();
                QStringList subKeys = subObj.keys();
                for(int k=0; k<subKeys.size(); k++)
                {
                    QString subkey = subKeys[k];
                    QJsonValue value = subObj[subkey];
                    if(value.isString())
                    {
                        qDebug() << "   " << value.toString();
                    }
                }
                qDebug() << "}";

            }
        }

    }


}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //写json数据到文件
    writeJsonToFile("test.json");

    //读json文件
    readJsonFromFile("test.json");



    return a.exec();
}
