#include <hpp/util/pointer.hh>
#include <hpp/pinocchio/device.hh>
#include <hpp/constraints/matrix-view.hh>
#include <hpp/core/path.hh>

namespace hpp {
  namespace core {
    HPP_PREDEF_CLASS(Discretization);
    typedef boost::shared_ptr<Discretization> DiscretizationPtr_t;

    class Discretization
    {
      public:
        enum ComputationOption
        {   Position              = 1
          , Derivative            = 2
          , PositionAndDerivative = Position | Derivative
        };

        static DiscretizationPtr_t create (const PathPtr_t path, const DevicePtr_t device)
        {
          DiscretizationPtr_t ptr (new Discretization(path, device));
          ptr->init(ptr);
          return ptr;
        }

        /// \param time
        void compute (value_type time);

        inline bool addCenterOfMass (const CenterOfMassComputationPtr_t& c, int option)
        {
          return addCenterOfMass (c, (ComputationOption) option);
        }

        bool addCenterOfMass (const CenterOfMassComputationPtr_t& c, ComputationOption option);

        inline bool addOperationalFrame (const std::string& name, int option)
        {
          return addOperationalFrame (name, (ComputationOption) option);
        }

        bool addOperationalFrame (const std::string& name, ComputationOption option);

        /// \throw std::runtime_error if a joint is not found in the model.
        void setJointNames (const std::vector<std::string>& names);

      private:
        Discretization (const PathPtr_t path, const DevicePtr_t device)
          : path_ (path), device_ (device)
        {}

        void init (const DiscretizationWkPtr_t)
        {}

        PathPtr_t path_;
        DevicePtr_t device_;

        Configuration_t q_;
        vector_t v_;

        Eigen::RowBlockIndices qView_, vView_;

        struct COM {
          CenterOfMassComputationPtr_t com;
          ComputationOption option;
          COM (CenterOfMassComputationPtr_t _com, ComputationOption _option)
            : com(_com), option(_option) {}
          void compute(pinocchio::DeviceData& d);
        };
        std::vector<COM> coms_;
        struct FrameData {
          pinocchio::FrameIndex index;
          ComputationOption option;
          FrameData (pinocchio::FrameIndex _index, ComputationOption _option)
            : index(_index), option(_option) {}
        };
        std::vector<FrameData> frames_;
    };
  } // namespace core
} // namespace hpp
