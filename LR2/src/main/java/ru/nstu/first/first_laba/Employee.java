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

    public Employee(Point2D position) {
        this.id = count++;
        this.position = position;
        this.random = new Random();
        this.name = generateName();
    }

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