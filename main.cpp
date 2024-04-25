#include <msclr/gcroot.h>
#using <System.dll>

using namespace msclr;
using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

const auto PATH = "COM3";
const int BAUDRATE = 9600;
const int INTERVAL = 1000;

// allow ref type static storage w/ gcroot()
gcroot<array<String^>^> info_line;
int padlen = 0;

int GetMaxLength(array<String^>^ split)
{
    int maxLength = 0;
    for each (auto str in split) {
        maxLength = std::max(str->Length, maxLength);
    }
    return maxLength;
}

void handleReceivedData(SerialPort^ port)
{
    String^ data = port->ReadLine();
    array<String^>^ split = data->Split(',');

    if (split[0] == "I")
    {
        if (split[1] == "Product ID")
        {
            info_line = split;
            padlen = GetMaxLength(split);
            Console::WriteLine(String::Join(",", info_line));
        }
        else
        {
            Console::WriteLine(split[3]);
        }
        return;
    }

    if (info_line == nullptr || info_line->Length < 1)
    {
        Console::WriteLine("Awaiting info line...");
        return;
    }

    String^ device = String::Format("{0} {1}", split[1], split[2]);
    array<String^>^ sensors = gcnew array<String^>((info_line->Length - 4) / 2);
    array<Double^>^ values = gcnew array<Double^>(sensors->Length);
    array<String^>^ units = gcnew array<String^>(sensors->Length);

    // map value positions into dedicated arrays
    for (int i = 0; i < info_line->Length - 5; i += 2) {
        sensors[i / 2] = info_line[i + 4]->Trim();
        values[i / 2] = Double::Parse(split[i + 4]);
        units[i / 2] = split[i + 5]->Trim();
    }

    // display results
    Console::WriteLine(String::Format("\n{0} {1} {2}", DateTime::Now.ToShortDateString(), DateTime::Now.ToShortTimeString(), device));
    for (int i = 0; i < units->Length; i++)
    {
        Console::WriteLine(String::Format("{0} {1} {2}", sensors[i]->PadRight(padlen + 2, ' '), values[i], units[i]));
    }
}

void Console_CancelKeyPress(Object^ sender, ConsoleCancelEventArgs^ e)
{
    Environment::Exit(0);
}

int main(array<String^>^ args)
{
    Console::CancelKeyPress += gcnew ConsoleCancelEventHandler(Console_CancelKeyPress);

    String^ path = gcnew String(PATH);
    SerialPort^ port = gcnew SerialPort(path, BAUDRATE, Parity::None, 8, StopBits::One);

    try
    {
        port->Open();

        port->WriteLine("POLL " + INTERVAL);
        Thread::Sleep(100);
        port->WriteLine("FRAC 2");
        Thread::Sleep(100);
        port->WriteLine("INFO");
        Thread::Sleep(100);

        // NOTE: while the standard approach would be to use Event handler `port.DataReceived`, it has
        // proven unable to receive non-input driven readout data thus far.
        while (port->IsOpen)
        {
            handleReceivedData(port);
        }
    }
    catch (Exception^ e)
    {
        Console::WriteLine(e->ToString());
    }
}
