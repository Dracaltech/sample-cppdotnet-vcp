#using <System.dll>
#using "System.Linq.dll"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO::Ports;
using namespace System::Linq;
using namespace System::Runtime::InteropServices;
using namespace System::Threading;

#include <msclr/gcroot.h>
using namespace msclr;

const auto PATH = "COM3";
const int BAUDRATE = 9600;
const int INTERVAL = 1000;

gcroot<array<String^>^> info_line;
int padlen = 0;

int GetMaxLength(array<String^>^ split)
{
    int maxLength = 0;
    for each (auto str in split) {
        maxLength = str->Length > maxLength ? str->Length : maxLength;
    }

    return maxLength;
}

array<String^>^ GetSensors(array<String^>^ info_line)
{
    List<String^>^ sensors = gcnew List<String^>();

    for (int i = 4; i < info_line->Length; i += 2)
    {
        sensors->Add(info_line[i]);
    }

    return sensors->ToArray();
}

array<String^>^ GetValues(array<String^>^ split)
{
    List<String^>^ values = gcnew List<String^>();

    for (int i = 0; i < split->Length; i += 2)
    {
        values->Add(split[i]);
    }

    return values->ToArray();
}

array<String^>^ GetUnits(array<String^>^ split)
{
    List<String^>^ units = gcnew List<String^>();

    for (int i = 1; i < split->Length; i += 2)
    {
        units->Add(split[i]);
    }

    return units->ToArray();
}

String^ GetCurrentTime()
{
    DateTime currentTime = DateTime::Now;
    return String::Format("{0} {1}", currentTime.ToShortDateString(), currentTime.ToShortTimeString());
}

void handleReceivedData(SerialPort^ port)
{
    String^ data = port->ReadLine();
    //List<String^>^ split = Enumerable::ToList(safe_cast<IEnumerable<String^>^>(data->Split(',')));
    array<String^>^ split = data->Split(',');

    if (split[0] == "I")
    {
        if (split[1] == "Product ID")
        {
            //TODO
            info_line = split;
            //TODO
            padlen = GetMaxLength(split);
            //TODO
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

    for (int i = 0; i < info_line->Length - 5; i += 2) {
        sensors[i / 2] = info_line[i + 4]->Trim();
        values[i / 2] = Double::Parse(split[i + 4]);
        units[i / 2] = split[i + 5]->Trim();
    }

    /*String^ device = info_line[4] + " " + info_line[5];
    array<String^>^ sensors = GetSensors(info_line);
    array<String^>^ values = GetValues(split);
    array<String^>^ units = GetUnits(split);*/

    Console::WriteLine(GetCurrentTime() + " " + device);
    for (int i = 0; i < units->Length; i++)
    {
        Console::WriteLine(String::Format("{0} {1} {2}", sensors[i]->PadLeft(padlen + 2, ' '), values[i], units[i]));
    }
    Console::WriteLine("\n");
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
