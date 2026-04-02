package ru.nstu.first.first_laba;

import java.util.ArrayList;
import java.util.List;

public class DataStorage {
    private static DataStorage instance;
    private List<Employee> employees;
    private int totalDevelopers;
    private int totalManagers;
    private long simulationTime;

    private DataStorage() {
        this.employees = new ArrayList<>();
        this.totalDevelopers = 0;
        this.totalManagers = 0;
        this.simulationTime = 0;
    }

    public static synchronized DataStorage getInstance() {
        if (instance == null) {
            instance = new DataStorage();
        }
        return instance;
    }

    public List<Employee> getEmployees() {
        return employees;
    }

    public void setEmployees(List<Employee> employees) {
        this.employees = new ArrayList<>(employees);
    }

    public int getTotalDevelopers() {
        return totalDevelopers;
    }

    public void setTotalDevelopers(int totalDevelopers) {
        this.totalDevelopers = totalDevelopers;
    }

    public int getTotalManagers() {
        return totalManagers;
    }

    public void setTotalManagers(int totalManagers) {
        this.totalManagers = totalManagers;
    }

    public long getSimulationTime() {
        return simulationTime;
    }

    public void setSimulationTime(long simulationTime) {
        this.simulationTime = simulationTime;
    }

    public void clear() {
        employees.clear();
        totalDevelopers = 0;
        totalManagers = 0;
        simulationTime = 0;
    }
}