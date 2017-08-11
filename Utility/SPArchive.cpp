#include "SPArchive.h"
namespace Pei
{
	namespace Utility
	{
		SPArchive::SPArchive(const char* filename, Pei::Utility::SPArchive::IOType ioType)
		{
			this->ioType = ioType;
			switch(ioType)
			{
			case IO_Output:
					this->totalBytes = 0;
					this->file = new std::fstream(filename,std::ios::out|std::ios::binary);
					break;
			case IO_Input:
					this->file = new std::fstream(filename,std::ios::in|std::ios::binary|std::ios::ate);
					this->totalBytes = this->file->tellg();
					this->file->seekg(0);
					break;
			}
		}
		SPArchive::~SPArchive()
		{
			this->Close();
			delete file;
		}
		void SPArchive::SaveBinary(void *mem, unsigned int size)
		{
			this->mutex.Lock();
			file->write((char*)mem,size);
			this->mutex.Unlock();
		}
		void SPArchive::LoadBinary(void *mem, unsigned int size)
		{
			this->mutex.Lock();
			file->read((char*)mem,size);
			this->mutex.Unlock();
		}
		void SPArchive::Close()
		{
			if(file->is_open())
				file->close();
		}
		unsigned int SPArchive::GetCurrentByte()
		{
			this->mutex.Lock();
			unsigned int cb = file->tellg();
			this->mutex.Unlock();
			return cb;
		}
		void Serialize(SPArchive& archive,int& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,bool& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,char& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,long& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,short& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,float& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,double& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,unsigned int& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,unsigned long& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,unsigned char& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,unsigned short& object){SERIALIZE_OBJECT(archive,object);}
		void Serialize(SPArchive& archive,SPSerializable& object)
		{
			object.serialize(archive);
			/*switch(archive.ioType)
			{
			case SPArchive::IO_Output:
					if(object.splitedSerialization)
						object.save(archive);
					else
					break;
			case SPArchive::IO_Input:
					if(object.splitedSerialization)
						object.load(archive);
					else
						object.serialize(archive);
					break;
			}*/
		}
		void Serialize(SPArchive& archive,std::string& object)
		{
			unsigned short size = 0;
			switch(archive.ioType)
			{
			case SPArchive::IO_Output:
					size = object.size();
					archive & size;
					archive.SaveBinary((void*)object.data(),size);
					break;
			case SPArchive::IO_Input:
					archive & size;
					char* buffer = new char[size+1];
					buffer[size] = '\0';
					archive.LoadBinary(buffer,size);
					object.assign(buffer,size);
					delete[] buffer;
					break;
			}
		}
	}
}