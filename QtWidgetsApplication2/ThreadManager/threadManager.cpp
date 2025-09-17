#include "threadmanager.h"
#include <QtConcurrent>
#include "visualization.h"

ThreadManager::ThreadManager(QObject* parent) : QObject(parent) {}

void ThreadManager::runVehicle(Vehicle* vehicle,
    const FindPath::Grid& grid,
    FindPath::Cell start,
    FindPath::Cell goal,
    QTableWidget* table,
    double speed,
    EnemyManager* enemies)  

{
    QtConcurrent::run([=]() {
        Visualization viz(table);

        double speed = 1.0;
        if (vehicle->name() == "Kara") speed = Speed::land;
        else if (vehicle->name() == "Deniz") speed = Speed::sea;
        else if (vehicle->name() == "Hava") speed = Speed::air;

        auto res = vehicle->findPath(grid, start, goal, &viz, speed,enemies);
        emit vehicleFinished(vehicle->name(), res);
        });
}
// ---------------- EnemyThread class constructor  ----------------
EnemyThread::EnemyThread(EnemyManager* manager, Visualization* viz, double speed)
    : m_manager(manager), m_viz(viz), m_speed(speed) {
}


void EnemyThread::run() {
    while (true) {
        // Eski düşman pozisyonlarını kaydet
        std::vector<FindPath::Cell> oldPositions;
        for (auto& e : m_manager->enemies()) {
            oldPositions.push_back(e.pos);
        }

        // Düşmanları hareket ettir
        m_manager->stepAll();

        if (m_viz && m_viz->table()) {
            auto table = m_viz->table();

            // 1. Eski pozisyonları temizle (ikon kaldır + zemin geri boya)
            for (auto& cell : oldPositions) {
                QTableWidgetItem* item = table->item(cell.r, cell.c);
                if (item) {
                    item->setIcon(QIcon());  // ikonu temizle

                    int value = m_manager->grid()[cell.r][cell.c];
                    // Eğer hücre zaten kara aracının yol rengi ile boyalıysa dokunma
                    if (item->background().color() != VisualizationConfig::LAND_COLOR) {
                        item->setBackground(VisualizationConfig::LAND_BG_COLOR);
                        item->setForeground(VisualizationConfig::LAND_TEXT_COLOR);
                        item->setText("Kara");
                    
                    }
                    else if (value == 1) { // Engel
                        item->setBackground(VisualizationConfig::OBSTACLE_COLOR);
                        item->setForeground(VisualizationConfig::OBSTACLE_TEXT_COLOR);
                        item->setText("Engel");
                    }
                    else if (value == 2) { // Deniz
                        item->setBackground(VisualizationConfig::SEA_COLOR);
                        item->setForeground(VisualizationConfig::SEA_TEXT_COLOR);
                        item->setText("Deniz");
                    }
                    else if (value == 3) { // Mayın
                        item->setBackground(VisualizationConfig::SEA_COLOR);
                        item->setForeground(VisualizationConfig::MINE_TEXT_COLOR);
                        item->setText("X");
                    }
                }
            }

            
            for (auto& e : m_manager->enemies()) {
                QTableWidgetItem* item = table->item(e.pos.r, e.pos.c);
                if (item) {
                    item->setIcon(VisualizationConfig::enemyIcon());
                }
            }
        }

        QCoreApplication::processEvents();
        QThread::msleep((int)(VisualizationConfig::STEP_DELAY_MS / m_speed));
    }
}
