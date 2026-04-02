package ru.nstu.first.first_laba;

import javafx.geometry.Dimension2D;
import javafx.geometry.Point2D;
import javafx.scene.canvas.GraphicsContext;

public class Managers extends Employee implements IBehaviour {
    private static final String[] NAMES = {"Олег", "Михаил", "Жанна", "Алексей", "Екатерина"};
    private int speed = 1;
    private double moveChance = 0.15; // 15% шанс движения

    public Managers(Point2D position) {
        super(position);
    }

    @Override
    protected String generateName() {
        return NAMES[random.nextInt(NAMES.length)] + "(MNG)";
    }

    @Override
    public void draw(GraphicsContext gc) {
        // Ничего не рисуем - картинка рисуется в Main
    }

    @Override
    public void update(int timeElapsed, Dimension2D area) {
        // Двигаемся только иногда
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
        return "Менеджер - проводит совещания";
    }
}