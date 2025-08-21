// main.cpp
#include <QtWidgets>
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QProgressBar>
#include <QSlider>
#include <QLabel>
#include <QGroupBox>
#include <QSplitter>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QTimer>
#include <QSystemTrayIcon>

class AdvancedMainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // Widgets principaux
    QTabWidget *centralTabs;
    QTableWidget *dataTable;
    QTreeWidget *hierarchyTree;
    QTextEdit *logOutput;
    
    // Contrôles
    QLineEdit *searchBox;
    QComboBox *categoryCombo;
    QProgressBar *progressBar;
    QSlider *volumeSlider;
    QLabel *statusLabel;
    
    // Actions et menus
    QAction *newAction, *openAction, *saveAction, *exitAction;
    QAction *aboutAction, *settingsAction;
    QToolBar *mainToolBar;
    QTimer *updateTimer;

public:
    AdvancedMainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setupUI();
        setupMenus();
        setupToolBar();
        setupStatusBar();
        setupConnections();
        setupTimer();
        
        setWindowTitle("Gestionnaire de Données Avancé");
        setWindowIcon(QIcon(":/icons/app.png"));
        resize(1200, 800);
    }

private slots:
    void onNewFile()
    {
        QMessageBox::information(this, "Nouveau", "Création d'un nouveau fichier...");
        progressBar->setValue(0);
        updateTimer->start(100);
    }
    
    void onOpenFile()
    {
        QString fileName = QFileDialog::getOpenFileName(this,
            "Ouvrir un fichier", "", "Tous les fichiers (*.*)");
        if (!fileName.isEmpty()) {
            logOutput->append("Fichier ouvert: " + fileName);
        }
    }
    
    void onSaveFile()
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            "Sauvegarder", "", "Fichiers de données (*.dat)");
        if (!fileName.isEmpty()) {
            logOutput->append("Fichier sauvegardé: " + fileName);
        }
    }
    
    void onSearch()
    {
        QString searchText = searchBox->text();
        if (!searchText.isEmpty()) {
            logOutput->append("Recherche: " + searchText);
            // Simulation de recherche dans le tableau
            for (int i = 0; i < dataTable->rowCount(); ++i) {
                for (int j = 0; j < dataTable->columnCount(); ++j) {
                    QTableWidgetItem *item = dataTable->item(i, j);
                    if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                        dataTable->selectRow(i);
                        logOutput->append("Trouvé dans ligne " + QString::number(i + 1));
                        return;
                    }
                }
            }
        }
    }
    
    void onCategoryChanged()
    {
        QString category = categoryCombo->currentText();
        logOutput->append("Catégorie changée: " + category);
        statusLabel->setText("Catégorie: " + category);
    }
    
    void onVolumeChanged(int value)
    {
        statusLabel->setText(QString("Volume: %1%").arg(value));
    }
    
    void updateProgress()
    {
        int value = progressBar->value();
        if (value < 100) {
            progressBar->setValue(value + 2);
        } else {
            updateTimer->stop();
            progressBar->setValue(0);
            QMessageBox::information(this, "Terminé", "Opération terminée avec succès!");
        }
    }
    
    void onAbout()
    {
        QMessageBox::about(this, "À propos",
            "Gestionnaire de Données Avancé v2.0\n"
            "Interface Qt moderne avec de nombreuses fonctionnalités\n"
            "Créé avec Qt 6");
    }
    
    void onSettings()
    {
        // Dialogue des paramètres
        QDialog settingsDialog(this);
        settingsDialog.setWindowTitle("Paramètres");
        settingsDialog.setModal(true);
        settingsDialog.resize(400, 300);
        
        QVBoxLayout *layout = new QVBoxLayout;
        
        // Choix de couleur
        QPushButton *colorButton = new QPushButton("Choisir couleur de fond");
        connect(colorButton, &QPushButton::clicked, [this]() {
            QColor color = QColorDialog::getColor(Qt::white, this, "Couleur de fond");
            if (color.isValid()) {
                setStyleSheet(QString("QMainWindow { background-color: %1; }").arg(color.name()));
            }
        });
        
        // Choix de police
        QPushButton *fontButton = new QPushButton("Choisir police");
        connect(fontButton, &QPushButton::clicked, [this]() {
            bool ok;
            QFont font = QFontDialog::getFont(&ok, this->font(), this, "Police");
            if (ok) {
                setFont(font);
            }
        });
        
        layout->addWidget(colorButton);
        layout->addWidget(fontButton);
        
        QPushButton *closeButton = new QPushButton("Fermer");
        connect(closeButton, &QPushButton::clicked, &settingsDialog, &QDialog::accept);
        layout->addWidget(closeButton);
        
        settingsDialog.setLayout(layout);
        settingsDialog.exec();
    }

private:
    void setupUI()
    {
        // Widget central avec onglets
        centralTabs = new QTabWidget;
        setCentralWidget(centralTabs);
        
        // Onglet 1: Tableau de données
        setupDataTab();
        
        // Onglet 2: Vue hiérarchique
        setupHierarchyTab();
        
        // Onglet 3: Contrôles avancés
        setupControlsTab();
        
        // Onglet 4: Logs
        setupLogTab();
    }
    
    void setupDataTab()
    {
        QWidget *dataWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(dataWidget);
        
        // Barre de recherche
        QHBoxLayout *searchLayout = new QHBoxLayout;
        searchBox = new QLineEdit;
        searchBox->setPlaceholderText("Rechercher...");
        QPushButton *searchButton = new QPushButton("Chercher");
        
        categoryCombo = new QComboBox;
        categoryCombo->addItems({"Tous", "Clients", "Produits", "Commandes", "Factures"});
        
        searchLayout->addWidget(new QLabel("Recherche:"));
        searchLayout->addWidget(searchBox);
        searchLayout->addWidget(searchButton);
        searchLayout->addWidget(new QLabel("Catégorie:"));
        searchLayout->addWidget(categoryCombo);
        searchLayout->addStretch();
        
        // Tableau de données
        dataTable = new QTableWidget(10, 6);
        QStringList headers = {"ID", "Nom", "Type", "Date", "Statut", "Valeur"};
        dataTable->setHorizontalHeaderLabels(headers);
        
        // Remplissage avec des données d'exemple
        for (int i = 0; i < 10; ++i) {
            dataTable->setItem(i, 0, new QTableWidgetItem(QString::number(1000 + i)));
            dataTable->setItem(i, 1, new QTableWidgetItem("Élément " + QString::number(i + 1)));
            dataTable->setItem(i, 2, new QTableWidgetItem(i % 2 ? "Type A" : "Type B"));
            dataTable->setItem(i, 3, new QTableWidgetItem("2024-01-" + QString("%1").arg(i + 1, 2, 10, QChar('0'))));
            dataTable->setItem(i, 4, new QTableWidgetItem(i % 3 ? "Actif" : "Inactif"));
            dataTable->setItem(i, 5, new QTableWidgetItem(QString::number((i + 1) * 100.5, 'f', 2)));
        }
        
        dataTable->resizeColumnsToContents();
        dataTable->setAlternatingRowColors(true);
        dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        
        layout->addLayout(searchLayout);
        layout->addWidget(dataTable);
        
        // Boutons d'action
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        QPushButton *addButton = new QPushButton("Ajouter");
        QPushButton *editButton = new QPushButton("Modifier");
        QPushButton *deleteButton = new QPushButton("Supprimer");
        
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(editButton);
        buttonLayout->addWidget(deleteButton);
        buttonLayout->addStretch();
        
        layout->addLayout(buttonLayout);
        
        centralTabs->addTab(dataWidget, "Données");
        
        // Connexions
        connect(searchButton, &QPushButton::clicked, this, &AdvancedMainWindow::onSearch);
        connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &AdvancedMainWindow::onCategoryChanged);
    }
    
    void setupHierarchyTab()
    {
        QWidget *hierarchyWidget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(hierarchyWidget);
        
        // Splitter pour diviser l'espace
        QSplitter *splitter = new QSplitter(Qt::Horizontal);
        
        // Arbre hiérarchique
        hierarchyTree = new QTreeWidget;
        hierarchyTree->setHeaderLabels({"Élément", "Type", "Valeur"});
        
        // Création de l'arbre d'exemple
        QTreeWidgetItem *rootItem = new QTreeWidgetItem(hierarchyTree);
        rootItem->setText(0, "Racine");
        rootItem->setText(1, "Dossier");
        rootItem->setText(2, "---");
        
        for (int i = 0; i < 3; ++i) {
            QTreeWidgetItem *categoryItem = new QTreeWidgetItem(rootItem);
            categoryItem->setText(0, "Catégorie " + QString::number(i + 1));
            categoryItem->setText(1, "Dossier");
            categoryItem->setText(2, QString::number((i + 1) * 10));
            
            for (int j = 0; j < 4; ++j) {
                QTreeWidgetItem *subItem = new QTreeWidgetItem(categoryItem);
                subItem->setText(0, "Élément " + QString::number(j + 1));
                subItem->setText(1, "Fichier");
                subItem->setText(2, QString::number((j + 1) * 5.5, 'f', 1));
            }
        }
        
        hierarchyTree->expandAll();
        
        // Panel de détails
        QWidget *detailsPanel = new QWidget;
        QVBoxLayout *detailsLayout = new QVBoxLayout(detailsPanel);
        
        detailsLayout->addWidget(new QLabel("Détails de l'élément sélectionné:"));
        
        QTextEdit *detailsText = new QTextEdit;
        detailsText->setPlainText("Sélectionnez un élément dans l'arbre pour voir ses détails...");
        detailsLayout->addWidget(detailsText);
        
        splitter->addWidget(hierarchyTree);
        splitter->addWidget(detailsPanel);
        splitter->setStretchFactor(0, 2);
        splitter->setStretchFactor(1, 1);
        
        layout->addWidget(splitter);
        
        centralTabs->addTab(hierarchyWidget, "Hiérarchie");
    }
    
    void setupControlsTab()
    {
        QWidget *controlsWidget = new QWidget;
        QVBoxLayout *mainLayout = new QVBoxLayout(controlsWidget);
        
        // Groupe de contrôles numériques
        QGroupBox *numericGroup = new QGroupBox("Contrôles Numériques");
        QGridLayout *numericLayout = new QGridLayout(numericGroup);
        
        QSpinBox *intSpinBox = new QSpinBox;
        intSpinBox->setRange(0, 1000);
        intSpinBox->setValue(50);
        
        QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox;
        doubleSpinBox->setRange(0.0, 100.0);
        doubleSpinBox->setDecimals(2);
        doubleSpinBox->setValue(25.75);
        
        volumeSlider = new QSlider(Qt::Horizontal);
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(75);
        
        progressBar = new QProgressBar;
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        
        numericLayout->addWidget(new QLabel("Entier:"), 0, 0);
        numericLayout->addWidget(intSpinBox, 0, 1);
        numericLayout->addWidget(new QLabel("Décimal:"), 1, 0);
        numericLayout->addWidget(doubleSpinBox, 1, 1);
        numericLayout->addWidget(new QLabel("Slider:"), 2, 0);
        numericLayout->addWidget(volumeSlider, 2, 1);
        numericLayout->addWidget(new QLabel("Progrès:"), 3, 0);
        numericLayout->addWidget(progressBar, 3, 1);
        
        // Groupe de contrôles de date/temps
        QGroupBox *dateGroup = new QGroupBox("Date et Temps");
        QGridLayout *dateLayout = new QGridLayout(dateGroup);
        
        QDateEdit *dateEdit = new QDateEdit(QDate::currentDate());
        QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime());
        QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
        
        dateLayout->addWidget(new QLabel("Date:"), 0, 0);
        dateLayout->addWidget(dateEdit, 0, 1);
        dateLayout->addWidget(new QLabel("Temps:"), 1, 0);
        dateLayout->addWidget(timeEdit, 1, 1);
        dateLayout->addWidget(new QLabel("Date/Temps:"), 2, 0);
        dateLayout->addWidget(dateTimeEdit, 2, 1);
        
        // Groupe de listes
        QGroupBox *listGroup = new QGroupBox("Listes et Sélections");
        QHBoxLayout *listLayout = new QHBoxLayout(listGroup);
        
        QListWidget *listWidget = new QListWidget;
        for (int i = 0; i < 10; ++i) {
            listWidget->addItem("Option " + QString::number(i + 1));
        }
        listWidget->setCurrentRow(0);
        
        QComboBox *comboBox = new QComboBox;
        comboBox->addItems({"Option A", "Option B", "Option C", "Option D"});
        comboBox->setEditable(true);
        
        QVBoxLayout *rightListLayout = new QVBoxLayout;
        rightListLayout->addWidget(new QLabel("ComboBox éditable:"));
        rightListLayout->addWidget(comboBox);
        rightListLayout->addStretch();
        
        listLayout->addWidget(listWidget);
        listLayout->addLayout(rightListLayout);
        
        mainLayout->addWidget(numericGroup);
        mainLayout->addWidget(dateGroup);
        mainLayout->addWidget(listGroup);
        
        centralTabs->addTab(controlsWidget, "Contrôles");
        
        // Connexions
        connect(volumeSlider, &QSlider::valueChanged, this, &AdvancedMainWindow::onVolumeChanged);
    }
    
    void setupLogTab()
    {
        QWidget *logWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(logWidget);
        
        // Contrôles de log
        QHBoxLayout *logControls = new QHBoxLayout;
        QPushButton *clearButton = new QPushButton("Effacer");
        QPushButton *saveLogButton = new QPushButton("Sauvegarder Log");
        QComboBox *logLevelCombo = new QComboBox;
        logLevelCombo->addItems({"Tous", "Info", "Attention", "Erreur"});
        
        logControls->addWidget(new QLabel("Niveau:"));
        logControls->addWidget(logLevelCombo);
        logControls->addStretch();
        logControls->addWidget(clearButton);
        logControls->addWidget(saveLogButton);
        
        // Zone de log
        logOutput = new QTextEdit;
        logOutput->setReadOnly(true);
        logOutput->setFont(QFont("Courier", 9));
        
        // Messages d'exemple
        logOutput->append("[INFO] Application démarrée");
        logOutput->append("[INFO] Interface utilisateur initialisée");
        logOutput->append("[ATTENTION] Configuration par défaut utilisée");
        
        layout->addLayout(logControls);
        layout->addWidget(logOutput);
        
        centralTabs->addTab(logWidget, "Logs");
        
        // Connexions
        connect(clearButton, &QPushButton::clicked, logOutput, &QTextEdit::clear);
    }
    
    void setupMenus()
    {
        // Menu Fichier
        QMenu *fileMenu = menuBar()->addMenu("Fichier");
        
        newAction = new QAction("Nouveau", this);
        newAction->setShortcut(QKeySequence::New);
        newAction->setStatusTip("Créer un nouveau fichier");
        
        openAction = new QAction("Ouvrir", this);
        openAction->setShortcut(QKeySequence::Open);
        openAction->setStatusTip("Ouvrir un fichier existant");
        
        saveAction = new QAction("Sauvegarder", this);
        saveAction->setShortcut(QKeySequence::Save);
        saveAction->setStatusTip("Sauvegarder le fichier actuel");
        
        exitAction = new QAction("Quitter", this);
        exitAction->setShortcut(QKeySequence::Quit);
        exitAction->setStatusTip("Quitter l'application");
        
        fileMenu->addAction(newAction);
        fileMenu->addAction(openAction);
        fileMenu->addAction(saveAction);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAction);
        
        // Menu Outils
        QMenu *toolsMenu = menuBar()->addMenu("Outils");
        
        settingsAction = new QAction("Paramètres", this);
        settingsAction->setStatusTip("Configurer l'application");
        
        toolsMenu->addAction(settingsAction);
        
        // Menu Aide
        QMenu *helpMenu = menuBar()->addMenu("Aide");
        
        aboutAction = new QAction("À propos", this);
        aboutAction->setStatusTip("À propos de cette application");
        
        helpMenu->addAction(aboutAction);
    }
    
    void setupToolBar()
    {
        mainToolBar = addToolBar("Principal");
        mainToolBar->addAction(newAction);
        mainToolBar->addAction(openAction);
        mainToolBar->addAction(saveAction);
        mainToolBar->addSeparator();
        
        // Ajout de widgets dans la toolbar
        mainToolBar->addWidget(new QLabel(" Recherche rapide: "));
        QLineEdit *quickSearch = new QLineEdit;
        quickSearch->setMaximumWidth(200);
        quickSearch->setPlaceholderText("Recherche rapide...");
        mainToolBar->addWidget(quickSearch);
    }
    
    void setupStatusBar()
    {
        statusLabel = new QLabel("Prêt");
        statusBar()->addWidget(statusLabel);
        
        // Indicateur de progression permanent
        QProgressBar *permProgressBar = new QProgressBar;
        permProgressBar->setMaximumWidth(150);
        permProgressBar->setVisible(false);
        statusBar()->addPermanentWidget(permProgressBar);
        
        // Label de statut permanent
        QLabel *permLabel = new QLabel("Connecté");
        statusBar()->addPermanentWidget(permLabel);
    }
    
    void setupConnections()
    {
        connect(newAction, &QAction::triggered, this, &AdvancedMainWindow::onNewFile);
        connect(openAction, &QAction::triggered, this, &AdvancedMainWindow::onOpenFile);
        connect(saveAction, &QAction::triggered, this, &AdvancedMainWindow::onSaveFile);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        connect(aboutAction, &QAction::triggered, this, &AdvancedMainWindow::onAbout);
        connect(settingsAction, &QAction::triggered, this, &AdvancedMainWindow::onSettings);
    }
    
    void setupTimer()
    {
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &AdvancedMainWindow::updateProgress);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Configuration de l'application
    app.setApplicationName("Gestionnaire de Données Avancé");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Mon Entreprise");
    
    // Style moderne
    app.setStyle("Fusion");
    
    // Palette sombre optionnelle
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    // app.setPalette(darkPalette); // Décommentez pour le thème sombre
    
    AdvancedMainWindow window;
    window.show();
    
    return app.exec();
}

#include "main.moc"