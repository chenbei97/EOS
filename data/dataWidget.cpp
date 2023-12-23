#include "dataWidget.h"
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQuickView>
#include <QQuickWindow>
#include <QQuickItem>
#include <QVBoxLayout>

dataWidget::dataWidget(QWidget* parent) : QWidget(parent)
{
//    QQmlApplicationEngine engine;
//    engine.addImportPath("qrc:/");
    //QQuickView quickView;

    //mMainView = new QQuickView;
    //mMainView->setSource(QUrl(QStringLiteral("qrc:/main.qml")));       //����QML
    //mMainView->setResizeMode(QQuickView::SizeRootObjectToView);
    //mMainView->show();                                                      //QQuickView������ʾ���ӻ�QML����
   //QQuickItem* mainQuickItem = mainView.rootObject();                    //���ص�ǰQQuickView�ĸ��ڵ�
   //QQuickWindow* mainWin = mainQuickItem->window();
   //mainWin->showFullScreen(); // ȫ����ʾ
}

QQuickView* dataWidget::GetMainQuickView()
{
    mMainView.setSource(QUrl("qrc:/main.qml"));       //����QML
    mMainView.setResizeMode(QQuickView::SizeRootObjectToView);
    mMainView.show();

    return &mMainView;
}

QQuickWidget* dataWidget::ShowMainView()
{
    mMainWidget = new QQuickWidget;
    mMainWidget->setSource(QUrl::fromLocalFile("qrc: /main1.qml"));
    mMainWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QQuickWindow* mainView = mMainWidget->quickWindow();
    qint32 childIndex = 0;
    if (mainView->contentItem()->childItems().size() > 1)
        childIndex = 1;


    auto contentChildItems = mainView->contentItem()->childItems();
    if (contentChildItems.size() > 0)
    {
        QQuickItem* mainQuickItem = contentChildItems.at(childIndex);
        if (!mainQuickItem)
        {
            qDebug() << "11111111";
        }
    }
    return mMainWidget;
}

QWidget* dataWidget::ShowView()
{
    mWidget = new QWidget;
    //mWidget->setStyleSheet("background-color:rgb(111,124,123);");
    auto s = new QVBoxLayout;

    mMainWidget = new QQuickWidget;
    mMainWidget->setSource(QUrl::fromLocalFile("qrc: /main.qml"));
    mMainWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    QQuickWindow* mainView = mMainWidget->quickWindow();
    qint32 childIndex = 0;
    if (mainView->contentItem()->childItems().size() > 1)
        childIndex = 1;


    auto contentChildItems = mainView->contentItem()->childItems();
    if (contentChildItems.size() > 0)
    {
        QQuickItem* mainQuickItem = contentChildItems.at(childIndex);
        if (!mainQuickItem)
        {
            qDebug() << "11111111";
        }
    }

    s->addWidget(mMainWidget);
    mWidget->setLayout(s);
    return mWidget;
}