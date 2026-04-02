package ru.nstu.first.first_laba;

import javafx.geometry.Dimension2D;
import javafx.geometry.Point2D;
import javafx.scene.canvas.GraphicsContext;
import java.util.Random;

public class Developers extends Employee implements IBehaviour {
    private static final String[] NAMES = {"Артем", "Даниил", "Александр", "Егор", "Максим"};
    private int speed = 1;
    private double moveChance = 0.2; // 20% шанс движения

    public Developers(Point2D position) {
        super(position);
    }

    @Override
    protected String generateName() {
        return NAMES[new Random().nextInt(NAMES.length)] + "(Dev)";
    }

    @Override
    public void draw(GraphicsContext gc) {
    }

    @Override
    public void update(int timeElapsed, Dimension2D area) {

        if (random.nextDouble() < moveChance) {
            int dx = random.nextInt(3) - 1;
            int dy = random.nextInt(3) - 1;

            double newX = position.getX() + dx * speed;
            double newY = position.getY() + dy * speed;

            if (newX > 40 && newX < area.getWidth() - 40) {
                position = new Point2D(newX, position.getY());
            }
            if (newY > 40 && newY < area.getHeight() - 40) {
                position = new Point2D(position.getX(), newY);
            }
        }
    }

    @Override
    public String getBehaviourType() {
        return "Разработчик - пишет код";
    }
}