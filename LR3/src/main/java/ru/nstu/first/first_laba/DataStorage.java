// DataStorage.java - только хранение данных
package ru.nstu.first.first_laba;

import java.util.*;

public class DataStorage {
    private static DataStorage instance;

    private LinkedList<Employee> employees;     // LinkedList для хранения объектов
    private TreeSet<Integer> activeIds;         // TreeSet для хранения ID
    private HashMap<Long, Employee> birthTimeMap; // HashMap для времени рождения
Object
    private int totalDevelopers;
    private int totalManagers;
    private long simulationTime;

    private DataStorage() {
        this.employees = new LinkedList<>();
        this.activeIds = new TreeSet<>();
        this.birthTimeMap = new HashMap<>();
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

    // Геттеры для коллекций
    public LinkedList<Employee> getEmployees() {
        return employees;
    }

    public TreeSet<Integer> getActiveIds() {
        return activeIds;
    }

    public HashMap<Long, Employee> getBirthTimeMap() {
        return birthTimeMap;
    }

    // Методы для работы с сотрудниками
    public void addEmployee(Employee emp) {
        employees.add(emp);
        activeIds.add(emp.getId());
        birthTimeMap.put(emp.getBirthTime(), emp);

        if (emp instanceof Developer) {
            totalDevelopers++;
        } else if (emp instanceof Manager) {
            totalManagers++;
        }
    }

    public void removeExpiredEmployees(long currentTime) {
        Iterator<Employee> iterator = employees.iterator();
        while (iterator.hasNext()) {
            Employee emp = iterator.next();
            if (emp.isExpired(currentTime)) {
                activeIds.remove(emp.getId());
                birthTimeMap.remove(emp.getBirthTime());
                iterator.remove();
                System.out.println("Удален: " + emp.getName() + " (ID: " + emp.getId() + ")");
            }
        }
    }

    public void clearAll() {
        employees.clear();
        activeIds.clear();
        birthTimeMap.clear();
        totalDevelopers = 0;
        totalManagers = 0;
        simulationTime = 0;
    }

    // Геттеры и сеттеры
    public int getTotalDevelopers() {
        return totalDevelopers;
    }

    public int getTotalManagers() {
        return totalManagers;
    }

    public void setSimulationTime(long simulationTime) {
        this.simulationTime = simulationTime;
    }
}