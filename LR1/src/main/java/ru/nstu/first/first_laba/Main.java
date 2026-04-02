package ru.nstu.first.first_laba;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.input.KeyCode;
import javafx.scene.layout.BorderPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

import java.io.InputStream;
import java.util.Objects;

public class Main extends Application {
    private Habitat habitat;
    private Canvas canvas;
    private GraphicsContext gc;
    private AnimationTimer timer;
    private long lastUpdate = 0;
    private boolean isRunning = false;
    private boolean showTime = true;
    private static final int WIDTH = 900;
    private static final int HEIGHT = 700;
    private static final int HABITAT_HEIGHT = 600;

    private Image developerImage;
    private Image managerImage;
    private Image backgroundImage;

    @Override
    public void start(Stage primaryStage) {
        this.habitat = new Habitat(WIDTH, HABITAT_HEIGHT);
        this.canvas = new Canvas(WIDTH, HEIGHT);
        this.gc = this.canvas.getGraphicsContext2D();

        loadImages();

        BorderPane root = new BorderPane();
        root.setCenter(this.canvas);

        Scene scene = new Scene(root, WIDTH, HEIGHT);

        scene.setOnKeyPressed(event -> {
            if (event.getCode() == KeyCode.B) {
                this.startSimulation();
            } else if (event.getCode() == KeyCode.E) {
                this.stopSimulation();
            } else if (event.getCode() == KeyCode.T) {
                this.showTime = !this.showTime;
                this.draw();
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
    }

    private void loadImages() {
        try {

            InputStream bgStream = getClass().getResourceAsStream("/back.jpg");
            InputStream devStream = getClass().getResourceAsStream("/DEV.png");
            InputStream mngStream = getClass().getResourceAsStream("/MNG.png");

            if (bgStream == null || devStream == null || mngStream == null) {
                System.out.println("Одно или нельколько изображений не найдены");
                return;
            }

            backgroundImage = new Image(bgStream);
            developerImage = new Image(devStream);
            managerImage = new Image(mngStream);

        } catch (Exception e) {
            System.err.println("Не удалось загрузить изображения: " + e.getMessage());
            developerImage = null;
            managerImage = null;
            backgroundImage = null;
        }
    }

    private void startSimulation() {
        this.isRunning = true;
        this.habitat.startSimulation();
    }

    private void stopSimulation() {
        this.isRunning = false;
        this.habitat.stopSimulation();
        this.draw();
    }

    private void draw() {
        if (backgroundImage != null && !backgroundImage.isError()) {
            gc.drawImage(backgroundImage, 0, 0, WIDTH, HEIGHT);
        } else {
            gc.setFill(Color.rgb(240, 248, 255));
            gc.fillRect(0, 0, WIDTH, HEIGHT);
        }

        if (this.isRunning) {
            for (Employee emp : this.habitat.getEmployees()) {
                if (emp instanceof Developers && developerImage != null && !developerImage.isError()) {
                    drawEmployeeWithImage(emp, developerImage, 60, 60);
                } else if (emp instanceof Managers && managerImage != null && !managerImage.isError()) {
                    drawEmployeeWithImage(emp, managerImage, 60, 60);
                }
            }

            if (this.showTime) {
                this.drawTime();
            }
        } else {
            this.drawStatistics();
        }

        this.drawHints();
    }

    private void drawEmployeeWithImage(Employee emp, Image image, double width, double height) {
        double x = emp.getPosition().getX() - width/2;
        double y = emp.getPosition().getY() - height/2;

        gc.drawImage(image, x, y, width, height);

        gc.setFill(Color.BLACK);
        gc.setFont(Font.font("Arial", FontWeight.BOLD, 12));
        gc.fillText(emp.getName(), x - 5, y + height + 15);
    }

    private void drawTime() {
        gc.setFill(Color.rgb(0, 0, 0, 0.7));
        gc.fillRoundRect(4, HABITAT_HEIGHT + 10, 130, 25, 5, 5);

        gc.setFill(Color.WHITE);
        gc.setFont(Font.font("Arial", 12));
        long timeSeconds = habitat.getCurrentTime() / 1000;
        String timeStr = String.format("Время: %d:%02d",
                timeSeconds / 60, timeSeconds % 60);
        gc.fillText(timeStr, 15, HABITAT_HEIGHT + 30);
    }

    private void drawStatistics() {
        int devCount = this.habitat.getTotalDevelopers();
        int manCount = this.habitat.getTotalManagers();

        long currentTimeMillis = this.habitat.getCurrentTime();
        long currentTimeSeconds = currentTimeMillis / 1000;

        gc.setFill(Color.rgb(255, 255, 255, 0.9));
        gc.fillRect(200, 80, 500, 320);
        gc.setStroke(Color.GRAY);
        gc.strokeRect(200, 80, 500, 320);

        gc.setFont(Font.font("Cambria", FontWeight.BOLD, 28));
        gc.setFill(Color.rgb(0, 102, 204));
        gc.fillText("СТАТИСТИКА", 380, 140);

        gc.setFont(Font.font("Consolas", FontWeight.BOLD, 18));
        gc.setFill(Color.rgb(102, 0, 153));
        gc.fillText(String.format("Время симуляции: %d мин %d сек",
                currentTimeSeconds / 60, currentTimeSeconds % 60), 250, 190);

        gc.setFont(Font.font("Verdana", FontWeight.BOLD, 20));
        gc.setFill(Color.rgb(70, 130, 180));
        gc.fillText("РАЗРАБОТЧИКИ", 250, 240);

        gc.setFont(Font.font("Verdana", 18));
        gc.fillText("Создано: " + devCount, 500, 240);

        gc.setFont(Font.font("Tahoma", FontWeight.BOLD, 20));
        gc.setFill(Color.rgb(255, 140, 0));
        gc.fillText("МЕНЕДЖЕРЫ", 250, 310);

        gc.setFont(Font.font("Tahoma", 18));
        gc.fillText("Создано: " + manCount, 500, 310);

        gc.setFont(Font.font("Georgia", FontWeight.BOLD, 18));
        gc.setFill(Color.rgb(34, 139, 34));
        gc.fillText("Всего сотрудников: " + (devCount + manCount), 250, 380);
    }

    private void drawHints() {
        gc.setFill(Color.rgb(0, 0, 0, 0.7));
        gc.fillRect(5, HEIGHT - 25, 280, 20);

        gc.setFill(Color.WHITE);
        gc.setFont(Font.font("Arial", 12));
        gc.fillText("B - Старт | E - Стоп | T - Время вкл/выкл", 10, HEIGHT - 10);
    }

    public static void main(String[] args) {
        launch(args);
    }
}