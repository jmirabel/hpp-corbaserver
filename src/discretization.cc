#include "hpp/corbaserver/discretization.hh"

#include <hpp/util/timer.hh>
#include <hpp/pinocchio/joint.hh>
#include <hpp/pinocchio/center-of-mass-computation.hh>

namespace hpp {
  namespace core {
    HPP_DEFINE_TIMECOUNTER(discretization);

    void Discretization::COM::compute (pinocchio::DeviceData& d)
    {
      switch (option) {
        case Position:
          com->compute (d, pinocchio::COM);
          break;
        case Derivative:
          com->compute (d, pinocchio::VELOCITY);
          break;
        case PositionAndDerivative:
          com->compute (d, pinocchio::COMPUTE_ALL);
          break;
      }
    }

    void Discretization::compute (value_type time)
    {
      if (!path_)
        throw std::logic_error ("Path is not set");
      HPP_START_TIMECOUNTER(discretization);

      q_.resize(device_->configSize());
      v_.resize(device_->numberDof ());

      bool success = path_->eval (q_, time);
      if (!success)
        throw std::runtime_error ("Could not evaluate the path");
      path_->derivative (v_, time, 1);

      pinocchio::DeviceSync device (device_);
      device.currentConfiguration(q_);
      device.currentVelocity     (v_);
      device.computeFramesForwardKinematics();

      for (std::size_t i = 0; i < coms_.size(); ++i)
      {
        coms_[i].compute(device.d());
        // publish it.
      }

      HPP_STOP_TIMECOUNTER(discretization);
      HPP_DISPLAY_LAST_TIMECOUNTER(discretization);
      HPP_DISPLAY_TIMECOUNTER(discretization);
    }

    bool Discretization::addCenterOfMass (
        const CenterOfMassComputationPtr_t& c, ComputationOption option)
    {
      for (std::size_t i = 0; i < coms_.size(); ++i)
        if (coms_[i].com == c) {
          coms_[i].option = (ComputationOption)(coms_[i].option | option);
          return true;
        }

      coms_.push_back(COM(c, option));
      return true;
    }

    bool Discretization::addOperationalFrame (
        const std::string& name, ComputationOption option)
    {
      const pinocchio::Model& model = device_->model();
      if (!model.existFrame (name)) return false;

      pinocchio::FrameIndex index = model.getFrameId(name);
      for (std::size_t i = 0; i < frames_.size(); ++i)
        if (frames_[i].index == index) {
          frames_[i].option = (ComputationOption)(frames_[i].option | option);
          return true;
        }

      frames_.push_back(FrameData(index, option));
      return true;
    }

    void Discretization::setJointNames (const std::vector<std::string>& names)
    {
      qView_ = Eigen::RowBlockIndices();
      vView_ = Eigen::RowBlockIndices();
      for (std::size_t i = 0; i < names.size(); ++i) {
        const std::string& name = names[i];
        JointPtr_t joint = device_->getJointByName(name);
        qView_.addRow(joint->rankInConfiguration(), joint->configSize());
        vView_.addRow(joint->rankInVelocity     (), joint->numberDof ());
      }
      qView_.updateRows<true,true,true>();
      vView_.updateRows<true,true,true>();
    }
  } // namespace core
} // namespace hpp
