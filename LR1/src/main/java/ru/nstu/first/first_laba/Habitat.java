package ru.nstu.first.first_laba;

import javafx.geometry.Dimension2D;
import javafx.geometry.Point2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Habitat {
    private Dimension2D area;
    private List<Employee> employees;
    private Random random;

    private int n1 = 2; // время генерации разрабов
    private double p1 = 0.7; // вероятность генерации разрабов
    private int n2 = 3; // время генерации менаджеров
    private double k = 0.3; // условие спавна, завис. от числа разработчиков

    private long startTime;
    private long simulationTime = 0;
    private boolean isRunning;
    private int totalDevelopers = 0;
    private int totalManagers = 0;
    private long lastDevGeneration = 0;
    private long lastManGeneration = 0;

    public Habitat(double width, double height) {
        this.area = new Dimension2D(width, height);
        this.employees = new ArrayList<>();
        this.random = new Random();
    }

    public void update(int timeElapsed) {
        if (!isRunning) return;

        simulationTime += timeElapsed;
        long currentTime = simulationTime / 1000;

        if (currentTime > lastDevGeneration && currentTime % n1 == 0 && random.nextDouble() < p1) {
            generateDeveloper();
            lastDevGeneration = currentTime;
        }

        if (currentTime > lastManGeneration && currentTime % n2 == 0) {
            long devCount = employees.stream().filter(e -> e instanceof Developers).count();
            long managerCount = employees.stream().filter(e -> e instanceof Managers).count();

            if (devCount > 0 && managerCount < devCount * k) {
                generateManager();
                lastManGeneration = currentTime;
            }
        }

        List<Employee> employeesCopy = new ArrayList<>(employees);
        for (Employee emp : employeesCopy) {
            if (emp instanceof IBehaviour) {
                ((IBehaviour) emp).update(timeElapsed, area);
            }
        }
    }

    private void generateDeveloper() {
        employees.add(new Developers(getRandomPosition()));
        totalDevelopers++;
    }

    private void generateManager() {
        employees.add(new Managers(getRandomPosition()));
        totalManagers++;
    }

    private Point2D getRandomPosition() {
        return new Point2D(
                50 + random.nextInt(Math.max(1, (int)area.getWidth() - 100)),
                50 + random.nextInt(Math.max(1, (int)area.getHeight() - 100))
        );
    }

    public void startSimulation() {
        employees.clear();
        startTime = System.currentTimeMillis();
        simulationTime = 0;
        isRunning = true;
        totalDevelopers = 0;
        totalManagers = 0;
        lastDevGeneration = 0;
        lastManGeneration = 0;
    }

    public void stopSimulation() {
        isRunning = false;
    }

    public boolean isRunning() {
        return isRunning;
    }

    public List<Employee> getEmployees() {
        return employees;
    }

    public long getCurrentTime() {
        if (!isRunning) {
            return simulationTime;
        }
        return System.currentTimeMillis() - startTime;
    }

    public int getTotalDevelopers() {
        return totalDevelopers;
    }

    public int getTotalManagers() {
        return totalManagers;
    }

    public void setN1(int n1) { this.n1 = Math.max(1, n1); }
    public void setP1(double p1) { this.p1 = Math.min(1.0, Math.max(0.0, p1)); }
    public void setN2(int n2) { this.n2 = Math.max(1, n2); }
    public void setK(double k) { this.k = Math.min(1.0, Math.max(0.0, k)); }

    public void clear() {
        employees.clear();
        totalDevelopers = 0;
        totalManagers = 0;
        simulationTime = 0;
    }
}