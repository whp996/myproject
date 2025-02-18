#ifndef MANAGETASK_H
#define MANAGETASK_H

#include <QWidget>
#include <database.h>
#include <ClassList.h>
#include <led.h>
#include <uestcqcustomplot.h>
#include <iostream>
#include <executetask.h>
#include <memory>
#include <QMainWindow>
#include <QStatusBar>
#include "irimagedisplay.h"
#include "IconImage.h"
#include "camera.h"
#include <QTreeWidgetItem>
#include <QSurfaceFormat>
#include <QOpenGLWidget>
#include "pxie8902.h"
#include <QSpinBox>
#include "labelediting.h"
#include "deviceid.h"
#include "taskconnectwire.h"
#include "FolderCheck.h"
#include "ch340.h"


namespace Ui {
class ManageTask;
}

class ManageTask : public QWidget
{
    Q_OBJECT

public:
    explicit ManageTask(QMainWindow *parent = nullptr);
    ~ManageTask();
    void MainWindowchange(QEvent *event);

    void deleteImage(const QString &imageId);

    void ConnectWire(QString step_id);
private slots:
    void displayWaveform(const QString& waveformid, const QString& path, const int deviceType);

    void undisplayWaveform(const QString& waveformid, const QString& tableName, const int deviceType);

    void on_pbViewTask_clicked();

    void on_pdRuntask_clicked();

    void on_pbInterrupt_clicked();

    void on_pbnextstep_clicked();

    void on_showOrHideBtn_clicked();

    void on_showOrHideBtnimage_clicked();

    void on_cbdeviceid_activated(int index);

    void on_pbrefresh_clicked();

    void on_comboBox_taskid_activated(int index);

    void saveinfraredframe(const QImage& image, uint16_t *tempData, uint32_t tempWidth, uint32_t tempHeight);

    void on_pblabelinfoshow_clicked();

    void GetInfraredImage(Step step);

    void on_pb8902expand_clicked();

    void on_pb5322expand_clicked();

    void on_pb5323expand_clicked();

    void saveinfraredframe_hypertherm(QImage image, std::vector<uint16_t> tempData, uint32_t tempWidth, uint32_t tempHeight);

signals:
    void UpdateMysqlDataSignal();

private:
    Ui::ManageTask *ui;
    QTimer *timer;
    Database database;
    std::vector<TestTask> currentTasks;
    QString currentTaskId;
    QString &device_id;
    QString current_run_task_id;
    QString current_run_task_table;
    Image IRimage;
    Step current_step;
    int element_id;
    std::vector<LED*> leds;
    std::shared_ptr<ExecuteTask> localExecuteTask;
    std::shared_ptr<QThread> taskThread;
    std::shared_ptr<LabelEditingWindow> labelediting;
    // std::unique_ptr<PXIe8902> pxie8902;
    std::vector<std::vector<float>> displaydata5322;
    std::vector<std::vector<float>> displaydata5323;
    std::vector<std::vector<float>> displaydata8902;
    std::vector<PXIe5711Waveform> display5711;
    std::vector<QString> displayid5322;
    std::vector<QString> displayid5323;
    std::vector<QString> displayid8902;
    std::vector<QString> displayid5711;
    std::unordered_map<QString, QString> display_time_id_map;

    UESTCQCustomPlot *customPlot5322;
    UESTCQCustomPlot *customPlot5323;
    UESTCQCustomPlot *customPlot8902;
    QWidget *plot8902Window = nullptr;
    QWidget *plot5322Window = nullptr;
    QWidget *plot5323Window = nullptr;
    QLabel *placeholderLabel8902 = nullptr;
    QLabel *placeholderLabel5322 = nullptr;
    QLabel *placeholderLabel5323 = nullptr;
    std::vector<QCPGraph*> graph8902;
    std::vector<QCPGraph*> graph5322;
    std::vector<QCPGraph*> graph5323;
    std::unordered_map<QString, QCPGraph*> path_graph_map;
    QMainWindow *parent;

    bool isTaskRunning = false;
    bool unfold = true;
    bool unfold_image = true;

    IRImageDisplay *irimagedisplay;
    IRImageDisplay *irimagedisplay_temp;
    IRImageDisplay *irimagedisplay_hypertherm = nullptr;
    QStackedWidget *stackedWidget;
    std::shared_ptr<TaskConnectWire> taskConnectWire;
    QStackedWidget *stackedWidget_connectwire;
    Camera& infraredcamera;
    bool is_infraredcamera_save = false;
    int step_collectedtime = 0;

    QImage LabeledingImage;
    std::vector<Label> label_info;
    std::vector<Label> label_info_add;
    std::vector<int> delete_id;

    void UpdateDisplay();
    void ComboBoxInit();
    void setupTreeWidget(const std::vector<Step>& steps);
    void setupStepContextMenu(QTreeWidgetItem *item, const QString &tableName);
    void toggleLegendDisplay();
    void StateChanged(const QString& state, int step);
    void initializeCharts();
    void updateTaskComboBox();
    void populateStepItem(QTreeWidgetItem* stepItem, const Step& step, const QString& tableName);
    void addWaveformsToTree(QTreeWidgetItem* parentItem, const std::vector<PXIe5320Waveform>& waveforms, int deviceType, const QString& tableName);
    void addWaveforms8902ToTree(QTreeWidgetItem* parentItem, const std::vector<Data8902>& waveforms8902, const QString& tableName);
    void addWaveforms5711ToTree(QTreeWidgetItem* parentItem, const std::vector<PXIe5711Waveform>& waveforms5711);
    QTreeWidgetItem* createStepItem(const Step& step);
    void addIRToTree(QTreeWidgetItem* parentItem, const Step& step, const QString& TableName);
    QTreeWidgetItem* createIRItem(const QString& TableName, const QString& stepId);
    void connectIRItem(QTreeWidgetItem* item);
    void handleIRItemChange(const QString& tableName, const QString& stepId);
    void connectWaveformItem(QTreeWidgetItem* item);
    void handleWaveformItemChange(QTreeWidgetItem* item);
    QTreeWidgetItem* createWaveformItem(const PXIe5320Waveform& waveform, const QString& tableName);
    QTreeWidgetItem* createWaveform8902Item(const Data8902& waveform8902, const QString& tableName);

    void handlePlotExpand(QWidget*& plotWindow, UESTCQCustomPlot* customPlot, QLabel*& placeholderLabel, 
                                 QScrollBar* scrollBar, QGridLayout* gridLayout, const QString& title);
    void handlePlotClose(QWidget*& plotWindow);
    void reparentPlot8902();
    void reparentPlot5322();
    void reparentPlot5323();
    void handlePlotReparent(UESTCQCustomPlot* customPlot, QLabel*& placeholderLabel,
                                   QScrollBar* scrollBar, QGridLayout* gridLayout,
                                   QVBoxLayout* verticalLayout);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MANAGETASK_H
