package ru.nstu.first.first_laba;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.InputStream;
import java.util.Optional;

public class Main extends Application {
    private Habitat habitat;
    private Canvas canvas;
    private GraphicsContext gc;
    private AnimationTimer timer;
    private long lastUpdate = 0;
    private boolean isRunning = false;
    private boolean showTime = true;
    private boolean showDialogOnStop = true;
    private static final int WIDTH = 1200;
    private static final int HEIGHT = 700;
    private static final int CANVAS_WIDTH = 800;
    private static final int CANVAS_HEIGHT = 500;
    private static final int CONTROL_PANEL_WIDTH = 400;

    private Image developerImage;
    private Image managerImage;
    private Image backgroundImage;

    private Button startButton;
    private Button stopButton;
    private Button startToolBtn;
    private Button stopToolBtn;
    private Label statusLabel;
    private CheckBox showDialogCheckBox;
    private RadioButton showTimeRadio;
    private RadioButton hideTimeRadio;
    private ToggleGroup timeToggleGroup;
    private TextField n1Field;
    private TextField n2Field;
    private ComboBox<Double> p1Combo;
    private ListView<Double> kList;
    private Label devCountLabel;
    private Label manCountLabel;
    private Label timeLabel;
    private MenuItem startMenuItem;
    private MenuItem stopMenuItem;
    private CheckMenuItem showTimeMenuItem;
    private CheckMenuItem showDialogMenuItem;

    @Override
    public void start(Stage primaryStage) {
        this.habitat = new Habitat(CANVAS_WIDTH, CANVAS_HEIGHT);
        this.canvas = new Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
        this.gc = this.canvas.getGraphicsContext2D();

        loadImages();

        BorderPane root = new BorderPane();

        VBox controlPanel = createControlPanel();

        StackPane canvasContainer = new StackPane(canvas);
        canvasContainer.setStyle("-fx-border-color: gray; -fx-border-width: 2;");

        SplitPane splitPane = new SplitPane();
        splitPane.getItems().addAll(canvasContainer, controlPanel);
        splitPane.setDividerPositions(0.67);

        root.setCenter(splitPane);

        MenuBar menuBar = createMenuBar(primaryStage);
        root.setTop(menuBar);

        ToolBar toolBar = createToolBar();
        root.setBottom(toolBar);

        Scene scene = new Scene(root, WIDTH, HEIGHT);

        scene.setOnKeyPressed(event -> {
            if (event.getCode() == KeyCode.B && !isRunning) {
                startSimulation();
            } else if (event.getCode() == KeyCode.E && isRunning) {
                stopSimulation();
            } else if (event.getCode() == KeyCode.T) {
                toggleTimeDisplay();
            }
        });

        this.timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                if (lastUpdate == 0) {
                    lastUpdate = now;
                    return;
                }

                if (isRunning) {
                    long deltaTime = (now - lastUpdate) / 1_000_000;
                    habitat.update((int) deltaTime);
                    updateStatistics();
                }

                draw();
                lastUpdate = now;
            }
        };
        this.timer.start();

        primaryStage.setTitle("Симуляция рабочего коллектива");
        primaryStage.setScene(scene);
        primaryStage.show();
        this.draw();
        updateUIState();
    }

    private VBox createControlPanel() {
        VBox panel = new VBox(15);
        panel.setPadding(new Insets(20));
        panel.setStyle("-fx-background-color: #f0f0f0;");
        panel.setPrefWidth(CONTROL_PANEL_WIDTH);

        Label titleLabel = new Label("Панель управления");
        titleLabel.setFont(Font.font("Arial", FontWeight.BOLD, 18));
        titleLabel.setAlignment(Pos.CENTER);
        titleLabel.setMaxWidth(Double.MAX_VALUE);

        HBox buttonBox = new HBox(10);
        buttonBox.setAlignment(Pos.CENTER);

        startButton = new Button("Старт");
        startButton.setPrefWidth(100);
        startButton.setOnAction(e -> startSimulation());

        stopButton = new Button("Стоп");
        stopButton.setPrefWidth(100);
        stopButton.setOnAction(e -> stopSimulation());
        stopButton.setDisable(true);

        buttonBox.getChildren().addAll(startButton, stopButton);

        showDialogCheckBox = new CheckBox("Показывать информацию при остановке");
        showDialogCheckBox.setSelected(true);
        showDialogCheckBox.setOnAction(e -> showDialogOnStop = showDialogCheckBox.isSelected());

        Label timeToggleLabel = new Label("Отображение времени:");
        timeToggleGroup = new ToggleGroup();

        showTimeRadio = new RadioButton("Показывать время");
        showTimeRadio.setToggleGroup(timeToggleGroup);
        showTimeRadio.setSelected(true);
        showTimeRadio.setOnAction(e -> {
            showTime = true;
            draw();
        });

        hideTimeRadio = new RadioButton("Скрывать время");
        hideTimeRadio.setToggleGroup(timeToggleGroup);
        hideTimeRadio.setOnAction(e -> {
            showTime = false;
            draw();
        });

        VBox timeBox = new VBox(5);
        timeBox.getChildren().addAll(timeToggleLabel, showTimeRadio, hideTimeRadio);
        timeBox.setPadding(new Insets(10, 0, 10, 20));
        timeBox.setStyle("-fx-border-color: #cccccc; -fx-border-width: 1; -fx-border-radius: 5;");

        TitledPane paramsPane = createParametersPane();
        TitledPane statsPane = createStatisticsPane();

        panel.getChildren().addAll(
                titleLabel,
                new Separator(),
                buttonBox,
                new Separator(),
                showDialogCheckBox,
                timeBox,
                paramsPane,
                statsPane
        );

        return panel;
    }

    private TitledPane createParametersPane() {
        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(10));

        grid.add(new Label("Период разработчиков (n1):"), 0, 0);
        n1Field = new TextField("2");
        n1Field.setPrefWidth(60);
        n1Field.textProperty().addListener((obs, old, newVal) -> {
            try {
                int value = Integer.parseInt(newVal);
                habitat.setN1(value);
            } catch (NumberFormatException e) {
                showErrorDialog("Ошибка ввода", "Введите целое число");
                n1Field.setText("2");
            }
        });
        grid.add(n1Field, 1, 0);

        grid.add(new Label("Вероятность p1:"), 0, 1);
        p1Combo = new ComboBox<>();
        p1Combo.getItems().addAll(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
        p1Combo.setValue(0.7);
        p1Combo.setOnAction(e -> habitat.setP1(p1Combo.getValue()));
        grid.add(p1Combo, 1, 1);

        grid.add(new Label("Период менеджеров (n2):"), 0, 2);
        n2Field = new TextField("3");
        n2Field.setPrefWidth(60);
        n2Field.textProperty().addListener((obs, old, newVal) -> {
            try {
                int value = Integer.parseInt(newVal);
                habitat.setN2(value);
            } catch (NumberFormatException e) {
                showErrorDialog("Ошибка ввода", "Введите целое число");
                n2Field.setText("3");
            }
        });
        grid.add(n2Field, 1, 2);

        grid.add(new Label("Коэффициент k:"), 0, 3);
        kList = new ListView<>();
        kList.getItems().addAll(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);
        kList.getSelectionModel().select(2);
        kList.getSelectionModel().selectedItemProperty().addListener((obs, old, newVal) -> {
            if (newVal != null) {
                habitat.setK(newVal);
            }
        });
        kList.setPrefHeight(100);
        grid.add(kList, 1, 3);

        TitledPane pane = new TitledPane("Параметры симуляции", grid);
        pane.setCollapsible(false);
        return pane;
    }

    private TitledPane createStatisticsPane() {
        GridPane grid = new GridPane();
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(10));

        grid.add(new Label("Разработчиков создано:"), 0, 0);
        devCountLabel = new Label("0");
        grid.add(devCountLabel, 1, 0);

        grid.add(new Label("Менеджеров создано:"), 0, 1);
        manCountLabel = new Label("0");
        grid.add(manCountLabel, 1, 1);

        grid.add(new Label("Время симуляции:"), 0, 2);
        timeLabel = new Label("0:00");
        grid.add(timeLabel, 1, 2);

        TitledPane pane = new TitledPane("Текущая статистика", grid);
        pane.setCollapsible(false);
        return pane;
    }

    private MenuBar createMenuBar(Stage primaryStage) {
        MenuBar menuBar = new MenuBar();

        Menu fileMenu = new Menu("Файл");
        startMenuItem = new MenuItem("Старт");
        startMenuItem.setAccelerator(javafx.scene.input.KeyCombination.keyCombination("B"));
        startMenuItem.setOnAction(e -> startSimulation());

        stopMenuItem = new MenuItem("Стоп");
        stopMenuItem.setAccelerator(javafx.scene.input.KeyCombination.keyCombination("E"));
        stopMenuItem.setOnAction(e -> stopSimulation());
        stopMenuItem.setDisable(true);

        MenuItem exitItem = new MenuItem("Выход");
        exitItem.setOnAction(e -> primaryStage.close());

        fileMenu.getItems().addAll(startMenuItem, stopMenuItem, new SeparatorMenuItem(), exitItem);

        Menu viewMenu = new Menu("Вид");
        showTimeMenuItem = new CheckMenuItem("Показывать время");
        showTimeMenuItem.setSelected(true);
        showTimeMenuItem.setAccelerator(javafx.scene.input.KeyCombination.keyCombination("T"));
        showTimeMenuItem.setOnAction(e -> {
            showTime = showTimeMenuItem.isSelected();
            if (showTime) showTimeRadio.setSelected(true);
            else hideTimeRadio.setSelected(true);
            draw();
        });

        showDialogMenuItem = new CheckMenuItem("Показывать диалог при остановке");
        showDialogMenuItem.setSelected(true);
        showDialogMenuItem.setOnAction(e -> showDialogOnStop = showDialogMenuItem.isSelected());

        viewMenu.getItems().addAll(showTimeMenuItem, showDialogMenuItem);

        Menu helpMenu = new Menu("Помощь");
        MenuItem aboutItem = new MenuItem("О программе");
        aboutItem.setOnAction(e -> showAboutDialog());
        helpMenu.getItems().add(aboutItem);

        menuBar.getMenus().addAll(fileMenu, viewMenu, helpMenu);
        return menuBar;
    }

    private ToolBar createToolBar() {
        startToolBtn = new Button("▶ Старт");
        startToolBtn.setOnAction(e -> startSimulation());

        stopToolBtn = new Button("■ Стоп");
        stopToolBtn.setOnAction(e -> stopSimulation());
        stopToolBtn.setDisable(true);

        Button timeToolBtn = new Button("⏱ Время");
        timeToolBtn.setOnAction(e -> toggleTimeDisplay());

        statusLabel = new Label("Симуляция остановлена");
        statusLabel.setTextFill(Color.RED);

        Region spacer1 = new Region();
        spacer1.setPrefWidth(10);
        Region spacer2 = new Region();
        spacer2.setPrefWidth(10);

        ToolBar toolBar = new ToolBar();
        toolBar.getItems().addAll(
                startToolBtn,
                stopToolBtn,
                spacer1,
                timeToolBtn,
                spacer2,
                statusLabel
        );

        return toolBar;
    }

    private void loadImages() {
        try {
            InputStream bgStream = getClass().getResourceAsStream("/back.jpg");
            InputStream devStream = getClass().getResourceAsStream("/DEV.png");
            InputStream mngStream = getClass().getResourceAsStream("/MNG.png");

            if (bgStream == null) {
                System.out.println("Фоновое изображение не найдено");
            }
            if (devStream == null) {
                System.out.println("Изображение разработчика не найдено");
            }
            if (mngStream == null) {
                System.out.println("Изображение менеджера не найдено");
            }

            if (bgStream != null) backgroundImage = new Image(bgStream);
            if (devStream != null) developerImage = new Image(devStream);
            if (mngStream != null) managerImage = new Image(mngStream);

        } catch (Exception e) {
            System.err.println("Не удалось загрузить изображения: " + e.getMessage());
        }
    }

    private void startSimulation() {
        this.isRunning = true;
        this.habitat.startSimulation();
        updateUIState();
        this.draw();
    }

    private void stopSimulation() {
        this.isRunning = false;
        this.habitat.pauseSimulation();
        updateUIState();
        this.draw();

        if (showDialogOnStop) {
            showSimulationDialog();
        }
    }

    private void resumeSimulation() {
        this.isRunning = true;
        this.habitat.resumeSimulation();
        updateUIState();
        this.draw();
    }

    private void toggleTimeDisplay() {
        this.showTime = !this.showTime;
        if (showTime) {
            showTimeRadio.setSelected(true);
        } else {
            hideTimeRadio.setSelected(true);
        }
        this.draw();
    }

    private void updateUIState() {
        // Панель управления
        startButton.setDisable(isRunning);
        stopButton.setDisable(!isRunning);

        // Панель инструментов
        if (startToolBtn != null) startToolBtn.setDisable(isRunning);
        if (stopToolBtn != null) stopToolBtn.setDisable(!isRunning);

        // Меню
        if (startMenuItem != null) startMenuItem.setDisable(isRunning);
        if (stopMenuItem != null) stopMenuItem.setDisable(!isRunning);

        // Статус
        updateStatusLabel();

        // Статистика
        updateStatistics();
    }

    private void updateStatusLabel() {
        if (statusLabel != null) {
            if (isRunning) {
                statusLabel.setText("Симуляция запущена");
                statusLabel.setTextFill(Color.GREEN);
            } else {
                statusLabel.setText("Симуляция остановлена");
                statusLabel.setTextFill(Color.RED);
            }
        }
    }

    private void updateStatistics() {
        devCountLabel.setText(String.valueOf(habitat.getTotalDevelopers()));
        manCountLabel.setText(String.valueOf(habitat.getTotalManagers()));
        long timeSeconds = habitat.getCurrentTime() / 1000;
        timeLabel.setText(String.format("%d:%02d", timeSeconds / 60, timeSeconds % 60));
    }

    private void showSimulationDialog() {
        Dialog<ButtonType> dialog = new Dialog<>();
        dialog.setTitle("Информация о симуляции");
        dialog.setHeaderText("Симуляция приостановлена");
        dialog.initModality(Modality.APPLICATION_MODAL);

        ButtonType okButton = new ButtonType("ОК", ButtonBar.ButtonData.OK_DONE);
        ButtonType cancelButton = new ButtonType("Отмена", ButtonBar.ButtonData.CANCEL_CLOSE);
        dialog.getDialogPane().getButtonTypes().addAll(okButton, cancelButton);

        VBox content = new VBox(10);
        content.setPadding(new Insets(20));

        TextArea textArea = new TextArea();
        textArea.setEditable(false);
        textArea.setPrefRowCount(10);
        textArea.setPrefWidth(400);

        long timeSeconds = habitat.getCurrentTime() / 1000;
        String info = String.format(
                "РЕЗУЛЬТАТЫ СИМУЛЯЦИИ\n" +
                        "====================\n\n" +
                        "Время симуляции: %d мин %d сек\n" +
                        "Всего создано разработчиков: %d\n" +
                        "Всего создано менеджеров: %d\n" +
                        "Всего сотрудников: %d\n\n" +
                        "Текущие параметры:\n" +
                        "n1 (период разработчиков): %s\n" +
                        "p1 (вероятность): %.1f\n" +
                        "n2 (период менеджеров): %s\n" +
                        "k (коэффициент): %.1f",
                timeSeconds / 60, timeSeconds % 60,
                habitat.getTotalDevelopers(),
                habitat.getTotalManagers(),
                habitat.getTotalDevelopers() + habitat.getTotalManagers(),
                n1Field.getText(),
                p1Combo.getValue(),
                n2Field.getText(),
                kList.getSelectionModel().getSelectedItem()
        );

        textArea.setText(info);
        content.getChildren().add(textArea);
        dialog.getDialogPane().setContent(content);

        Optional<ButtonType> result = dialog.showAndWait();

        if (result.isPresent() && result.get() == cancelButton) {
            // Нажали Отмена - продолжаем симуляцию
            resumeSimulation();
        } else {
            // Нажали ОК или закрыли окно - симуляция остаётся остановленной
            // Обновляем UI, чтобы убедиться, что всё в правильном состоянии
            updateUIState();
            this.draw();
        }
    }

    private void showErrorDialog(String title, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    private void showAboutDialog() {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("О программе");
        alert.setHeaderText("Симуляция рабочего коллектива");
        alert.setContentText(
                "Версия 2.0\n\n" +
                        "Разработчик: Студент группы ...\n" +
                        "Год: 2024\n\n" +
                        "Управление:\n" +
                        "B - Старт\n" +
                        "E - Стоп\n" +
                        "T - Показать/скрыть время"
        );
        alert.showAndWait();
    }

    private void draw() {
        // Рисуем фон
        if (backgroundImage != null && !backgroundImage.isError()) {
            gc.drawImage(backgroundImage, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
        } else {
            gc.setFill(Color.rgb(240, 248, 255));
            gc.fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);
        }

        // Рисуем сотрудников (всегда, даже когда симуляция на паузе)
        for (Employee emp : this.habitat.getEmployees()) {
            if (emp instanceof Developer && developerImage != null && !developerImage.isError()) {
                drawEmployeeWithImage(emp, developerImage, 50, 50);
            } else if (emp instanceof Manager && managerImage != null && !managerImage.isError()) {
                drawEmployeeWithImage(emp, managerImage, 50, 50);
            }
        }

        // Рисуем время, если нужно
        if (this.showTime) {
            this.drawTime();
        }
    }

    private void drawEmployeeWithImage(Employee emp, Image image, double width, double height) {
        double x = emp.getPosition().getX() - width/2;
        double y = emp.getPosition().getY() - height/2;
        gc.drawImage(image, x, y, width, height);

        gc.setFill(Color.BLACK);
        gc.setFont(Font.font("Arial", FontWeight.BOLD, 10));
        gc.fillText(emp.getName(), x - 5, y + height + 12);
    }

    private void drawTime() {
        gc.setFill(Color.rgb(0, 0, 0, 0.7));
        gc.fillRoundRect(10, 10, 120, 25, 5, 5);

        gc.setFill(Color.WHITE);
        gc.setFont(Font.font("Arial", 12));
        long timeSeconds = habitat.getCurrentTime() / 1000;
        String timeStr = String.format("Время: %d:%02d",
                timeSeconds / 60, timeSeconds % 60);
        gc.fillText(timeStr, 20, 28);
    }

    public static void main(String[] args) {
        launch(args);
    }
}