package ru.nstu.first.first_laba;

import javafx.geometry.Point2D;
import javafx.scene.canvas.GraphicsContext;
import java.util.Random;

public abstract class Employee {
    protected String name;
    protected Point2D position;
    protected int id;
    protected static int count = 0;
    protected Random random;
    protected long birthTime;
    protected long lifetime;
    protected boolean isAlive;

    // Статический счетчик для ID
    private static int nextId = 1;

    public Employee(Point2D position, long birthTime, long lifetime) {
        this.id = generateUniqueId();
        this.position = position;
        this.random = new Random();
        this.name = generateName();
        this.birthTime = birthTime;
        this.lifetime = lifetime;
        this.isAlive = true;
        count++;
    }

    // Правильный метод генерации ID
    private static synchronized int generateUniqueId() {
        return nextId++;
    }

    // Метод для сброса счетчика (можно добавить)
    public static void resetIdCounter() {
        nextId = 1;
    }

    public boolean isExpired(long currentTime) {
        return (currentTime - birthTime) >= lifetime;
    }

    public long getBirthTime() { return birthTime; }
    public long getLifetime() { return lifetime; }
    public void setLifetime(long lifetime) { this.lifetime = lifetime; }
    public boolean isAlive() { return isAlive; }
    public void setAlive(boolean alive) { isAlive = alive; }

    protected abstract String generateName();
    public abstract void draw(GraphicsContext gc);

    public Point2D getPosition() {
        return position;
    }

    public void setPosition(Point2D position) {
        this.position = position;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }
}