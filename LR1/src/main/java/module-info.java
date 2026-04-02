module ru.nstu.first.first_laba {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens ru.nstu.first.first_laba to javafx.fxml;
    exports ru.nstu.first.first_laba;
}