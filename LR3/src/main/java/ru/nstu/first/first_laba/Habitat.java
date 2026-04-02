// Habitat.java - управление симуляцией и бизнес-логика
package ru.nstu.first.first_laba;

import javafx.geometry.Dimension2D;
import javafx.geometry.Point2D;

import java.util.*;

public class Habitat {
    private Dimension2D area;
    private Random random;
    private DataStorage storage;

    private boolean isRunning;
    private long startTime;
    private long simulationTime;

    private int developerLifetime = 30000;
    private int managerLifetime = 45000;

    private int n1 = 2;
    private double p1 = 0.7;
    private int n2 = 3;
    private double k = 0.3;

    private long lastDevGeneration = 0;
    private long lastManGeneration = 0;

    public Habitat(double width, double height) {
        this.storage = DataStorage.getInstance();
        this.area = new Dimension2D(width, height);
        this.random = new Random();
        this.isRunning = false;
        this.startTime = 0;
        this.simulationTime = 0;
    }

    public void update(int timeElapsed) {
        if (!isRunning) return;

        long currentTime = getCurrentTime();

        // Удаляем просроченных сотрудников
        storage.removeExpiredEmployees(currentTime);

        long currentSeconds = currentTime / 1000;

        // Генерация разработчиков
        if (currentSeconds > lastDevGeneration && currentSeconds % n1 == 0 && random.nextDouble() < p1) {
            generateDeveloper(currentTime);
            lastDevGeneration = currentSeconds;
        }

        // Генерация менеджеров
        if (currentSeconds > lastManGeneration && currentSeconds % n2 == 0) {
            long devCount = storage.getEmployees().stream().filter(e -> e instanceof Developer).count();
            long managerCount = storage.getEmployees().stream().filter(e -> e instanceof Manager).count();

            if (devCount > 0 && managerCount < devCount * k) {
                generateManager(currentTime);
                lastManGeneration = currentSeconds;
            }
        }

        // Обновление позиций сотрудников
        List<Employee> employeesCopy = new ArrayList<>(storage.getEmployees());
        for (Employee emp : employeesCopy) {
            if (emp instanceof IBehaviour) {
                ((IBehaviour) emp).update(timeElapsed, area);
            }
        }

//        Iterator<Employee> iterator = storage.getEmployees().iterator();
//        while (iterator.hasNext()) {
//            Employee emp = iterator.next();
//            if (emp instanceof IBehaviour) {
//                ((IBehaviour) emp).update(timeElapsed, area);
//            }
//        }

        // Сохраняем время в storage для статистики
        storage.setSimulationTime(currentTime);
    }

    private void generateDeveloper(long currentTime) {
        Developer dev = new Developer(
                getRandomPosition(),
                currentTime,
                developerLifetime
        );
        storage.addEmployee(dev);
    }

    private void generateManager(long currentTime) {
        Manager man = new Manager(
                getRandomPosition(),
                currentTime,
                managerLifetime
        );
        storage.addEmployee(man);
    }

    private Point2D getRandomPosition() {
        return new Point2D(
                50 + random.nextInt(Math.max(1, (int) area.getWidth() - 100)),
                50 + random.nextInt(Math.max(1, (int) area.getHeight() - 100))
        );
    }

    // Методы управления симуляцией
    public void startSimulation() {
        storage.clearAll();
        Employee.resetIdCounter();
        startTime = System.currentTimeMillis();
        simulationTime = 0;
        isRunning = true;
        lastDevGeneration = 0;
        lastManGeneration = 0;
        storage.setSimulationTime(0);
    }

    public void pauseSimulation() {
        if (isRunning) {
            simulationTime = System.currentTimeMillis() - startTime;
            isRunning = false;
            storage.setSimulationTime(simulationTime);
        }
    }

    public void resumeSimulation() {
        if (!isRunning) {
            startTime = System.currentTimeMillis() - simulationTime;
            isRunning = true;
        }
    }

    public long getCurrentTime() {
        if (!isRunning) {
            return simulationTime;
        }
        return System.currentTimeMillis() - startTime;
    }

    public boolean isRunning() {
        return isRunning;
    }

    // Геттеры для UI (делегируем в storage)
    public LinkedList<Employee> getEmployees() {
        return storage.getEmployees();
    }

    public HashMap<Long, Employee> getBirthTimeMap() {
        return storage.getBirthTimeMap();
    }

    public TreeSet<Integer> getActiveIds() {
        return storage.getActiveIds();
    }

    public int getTotalDevelopers() {
        return storage.getTotalDevelopers();
    }

    public int getTotalManagers() {
        return storage.getTotalManagers();
    }

    // Сеттеры параметров
    public void setDeveloperLifetime(int lifetime) {
        this.developerLifetime = lifetime;
    }

    public void setManagerLifetime(int lifetime) {
        this.managerLifetime = lifetime;
    }

    public void setN1(int n1) {
        this.n1 = Math.max(1, n1);
    }

    public void setP1(double p1) {
        this.p1 = Math.min(1.0, Math.max(0.0, p1));
    }

    public void setN2(int n2) {
        this.n2 = Math.max(1, n2);
    }

    public void setK(double k) {
        this.k = Math.min(1.0, Math.max(0.0, k));
    }

    public int getDeveloperLifetime() {
        return developerLifetime;
    }

    public int getManagerLifetime() {
        return managerLifetime;
    }
}