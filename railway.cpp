#include <windows.h>
#include <string>
#include <vector>

using namespace std;

/* ================= DATA ================= */

struct Train {
    int number;
    string name;
    string source;
    string destination;
    int seats;
    int price;
};

struct Booking {
    int trainIndex;
    string passengerName;
    string phone;
    string date;
    bool active;
};

// Initial train data
vector<Train> trains = {
    {201, "Green Line Express", "Karachi", "Islamabad", 200, 4500},
    {202, "Karachi Express", "Karachi", "Lahore", 180, 4000},
    {203, "Allama Iqbal Express", "Lahore", "Karachi", 170, 4200},
    {204, "Tezgam Express", "Karachi", "Rawalpindi", 160, 4300},
    {205, "Awam Express", "Karachi", "Peshawar", 190, 4600}
};

// Track all bookings
vector<Booking> bookings;

/* ================= GLOBAL HANDLES ================= */

HWND hList;
HWND hName, hPhone, hDate;

/* ================= FUNCTIONS ================= */

string GetText(HWND hEdit) {
    char buffer[100];
    GetWindowText(hEdit, buffer, 100);
    return string(buffer);
}

void LoadTrains() {
    SendMessage(hList, LB_RESETCONTENT, 0, 0);
    for (auto &t : trains) {
        string line =
            to_string(t.number) + " | " +
            t.name + " | " +
            t.source + " -> " + t.destination +
            " | Available Seats: " + to_string(t.seats) +
            " | Rs. " + to_string(t.price);
        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)line.c_str());
    }
}

/* ================= WINDOW PROCEDURE ================= */

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {

    case WM_CREATE:
        // Title
        CreateWindow("STATIC", "Pakistan Railway Reservation System",
            WS_VISIBLE | WS_CHILD,
            20, 10, 450, 20,
            hwnd, NULL, NULL, NULL);

        // Listbox for trains
        hList = CreateWindow("LISTBOX", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_STANDARD,
            20, 40, 520, 160,
            hwnd, NULL, NULL, NULL);

        // Show Available Seats button
        CreateWindow("BUTTON", "Show Available Seats",
            WS_VISIBLE | WS_CHILD,
            20, 210, 160, 30,
            hwnd, (HMENU)5, NULL, NULL);

        // Passenger Name
        CreateWindow("STATIC", "Passenger Name:",
            WS_VISIBLE | WS_CHILD,
            20, 250, 120, 20,
            hwnd, NULL, NULL, NULL);

        hName = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            150, 250, 200, 20,
            hwnd, NULL, NULL, NULL);

        // Phone
        CreateWindow("STATIC", "Phone Number:",
            WS_VISIBLE | WS_CHILD,
            20, 280, 120, 20,
            hwnd, NULL, NULL, NULL);

        hPhone = CreateWindow("EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            150, 280, 200, 20,
            hwnd, NULL, NULL, NULL);

        // Date
        CreateWindow("STATIC", "Travel Date:",
            WS_VISIBLE | WS_CHILD,
            20, 310, 120, 20,
            hwnd, NULL, NULL, NULL);

        hDate = CreateWindow("EDIT", "DD/MM/YYYY",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            150, 310, 200, 20,
            hwnd, NULL, NULL, NULL);

        // Buttons
        CreateWindow("BUTTON", "Book Ticket",
            WS_VISIBLE | WS_CHILD,
            380, 250, 140, 30,
            hwnd, (HMENU)1, NULL, NULL);

        CreateWindow("BUTTON", "Book Another Ticket",
            WS_VISIBLE | WS_CHILD,
            380, 290, 140, 30,
            hwnd, (HMENU)4, NULL, NULL);

        CreateWindow("BUTTON", "Cancel Last Ticket",
            WS_VISIBLE | WS_CHILD,
            380, 330, 140, 30,
            hwnd, (HMENU)2, NULL, NULL);

        CreateWindow("BUTTON", "Exit",
            WS_VISIBLE | WS_CHILD,
            380, 370, 140, 30,
            hwnd, (HMENU)3, NULL, NULL);

        LoadTrains();
        break;

    case WM_COMMAND:
        {
            int selIndex = SendMessage(hList, LB_GETCURSEL, 0, 0);
            string name = GetText(hName);
            string phone = GetText(hPhone);
            string date = GetText(hDate);

            // SHOW AVAILABLE SEATS
            if (LOWORD(wp) == 5) {
                string seatsInfo = "Available Seats:\n\n";
                for (auto &t : trains) {
                    seatsInfo += t.name + " (" + to_string(t.number) + "): " +
                                 to_string(t.seats) + " seats\n";
                }
                MessageBox(hwnd, seatsInfo.c_str(), "Available Seats", MB_OK | MB_ICONINFORMATION);
            }

            // BOOK TICKET
            if (LOWORD(wp) == 1) {
                if (selIndex == LB_ERR || name.empty() || phone.empty() || date.empty()) {
                    MessageBox(hwnd, "Please fill all fields and select a train!", "Error", MB_OK | MB_ICONERROR);
                    break;
                }
                if (trains[selIndex].seats <= 0) {
                    MessageBox(hwnd, "No seats available!", "Error", MB_OK | MB_ICONERROR);
                    break;
                }

                trains[selIndex].seats--;
                Booking b = {selIndex, name, phone, date, true};
                bookings.push_back(b);

                string msg = "TICKET BOOKED SUCCESSFULLY\n\nPassenger: " + name +
                             "\nPhone: " + phone +
                             "\nDate: " + date +
                             "\n\nTrain: " + trains[selIndex].name +
                             "\nRoute: " + trains[selIndex].source + " -> " + trains[selIndex].destination +
                             "\nPrice: Rs. " + to_string(trains[selIndex].price);

                MessageBox(hwnd, msg.c_str(), "Booking Confirmed", MB_OK | MB_ICONINFORMATION);
                LoadTrains();
            }

            // BOOK ANOTHER TICKET
            if (LOWORD(wp) == 4) {
                if (selIndex == LB_ERR || name.empty() || phone.empty() || date.empty()) {
                    MessageBox(hwnd, "Please fill all fields and select a train!", "Error", MB_OK | MB_ICONERROR);
                    break;
                }
                if (trains[selIndex].seats <= 0) {
                    MessageBox(hwnd, "No seats available!", "Error", MB_OK | MB_ICONERROR);
                    break;
                }

                trains[selIndex].seats--;
                Booking b = {selIndex, name, phone, date, true};
                bookings.push_back(b);

                string msg = "ANOTHER TICKET BOOKED SUCCESSFULLY\n\nPassenger: " + name +
                             "\nPhone: " + phone +
                             "\nDate: " + date +
                             "\n\nTrain: " + trains[selIndex].name +
                             "\nRoute: " + trains[selIndex].source + " -> " + trains[selIndex].destination +
                             "\nPrice: Rs. " + to_string(trains[selIndex].price);

                MessageBox(hwnd, msg.c_str(), "Booking Confirmed", MB_OK | MB_ICONINFORMATION);
                LoadTrains();
            }

            // CANCEL LAST TICKET
            if (LOWORD(wp) == 2) {
                if (bookings.empty()) {
                    MessageBox(hwnd, "No active booking to cancel!", "Error", MB_OK | MB_ICONERROR);
                    break;
                }

                Booking last = bookings.back();
                if (last.active) {
                    trains[last.trainIndex].seats++;
                    bookings.back().active = false;

                    MessageBox(hwnd, "Ticket cancelled successfully!\nSeat restored.", "Cancellation", MB_OK | MB_ICONINFORMATION);
                    LoadTrains();
                }
            }

            // EXIT
            if (LOWORD(wp) == 3) {
                PostQuitMessage(0);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

/* ================= MAIN ================= */

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "RailwayGUI";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "RailwayGUI",
        "Pakistan Railway Reservation System",
        WS_OVERLAPPEDWINDOW,
        200, 200, 600, 450,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

