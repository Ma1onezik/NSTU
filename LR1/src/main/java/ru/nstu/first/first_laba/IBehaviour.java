package ru.nstu.first.first_laba;

import javafx.geometry.Dimension2D;

public interface IBehaviour {
    void update(int timeElapsed, Dimension2D area);
    String getBehaviourType();
}